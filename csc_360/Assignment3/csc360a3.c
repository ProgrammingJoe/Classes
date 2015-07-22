#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#define FILESIZE 4

typedef struct SuperBlock_Struct {
  uint32_t block_size;
  uint32_t block_count;
  uint32_t fat_start;
  uint32_t fat_blocks;
  uint32_t root_start;
  uint32_t root_blocks;
} SuperBlock_t;

typedef struct FatBlock_Struct {
  uint32_t free_blocks;
  uint32_t reserved_blocks;
  uint32_t allocated_blocks;
} FatBlock_t;
 

uint32_t get_super_info(char* mp, uint32_t offset, uint32_t length){
	int x = 0;
	uint32_t ret = 0;

	char* tmp = (char *)malloc(sizeof(char) * length);
	memcpy(tmp, mp+offset, length);
	
	for(x = 0; x < length; x++){
		ret += ((int)tmp[x]<<(8*(length - x - 1)));
	}
	free(tmp);
	return ret;
}

void parse_directory(char * directory, char * dirarr){

}

/*
Displays super block information

Displays fat block information
*/

#if defined(PART1)
void diskinfo(char ** argv){
	struct SuperBlock_Struct super;
	int file;
	struct stat sf;
	char *mp;
	if((file = open(argv[1], O_RDONLY))){
		fstat(file, &sf);
		mp = mmap(NULL, sf.st_size, PROT_READ, MAP_SHARED, file, 0);
		super.block_size = get_super_info(mp, 8, 2);
		super.block_count = get_super_info(mp, 10, 4);
		super.fat_start = get_super_info(mp, 14, 4);
		super.fat_blocks = get_super_info(mp, 18, 4);
		super.root_start = get_super_info(mp, 22, 4);
		super.root_blocks = get_super_info(mp, 26, 4);
	} else {
		printf("Failed to open the file\n");
	}
	
	printf("Super block information:\n");
	printf("Block size: %d\n", super.block_size);
	printf("Block count: %d\n", super.block_count);
	printf("FAT starts: %d\n", super.fat_start);
	printf("FAT blocks: %d\n", super.fat_blocks);
	printf("Root directory start: %d\n", super.root_start);
	printf("Root directory blocks: %d\n\n", super.root_blocks);

	struct FatBlock_Struct fat;
	int x = 0;
	for(x = 0; x < super.fat_blocks*super.block_size; x += 4){
		uint32_t value = 0;
		value = get_super_info(mp, super.fat_start*super.block_size + x, 4);
		if(value == 0){
			fat.free_blocks += 1;
		}else if(value == 1){
			fat.reserved_blocks += 1;
		}else{
			fat.allocated_blocks += 1;
		}
	}

	printf("FAT information: \n");
	printf("Free Blocks: %d\n", fat.free_blocks);
	printf("Reserved Blocks: %d\n", fat.reserved_blocks);
	printf("Allocated Blocks: %d\n", fat.allocated_blocks);
}
#endif


void print_time(char *file_modify_bytes){
	int x = 0;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	for(x = 0; x < 2; x++){
		year += (((int)file_modify_bytes[x] & 0xff) <<(8*(-x + 2 - 1)));
	}	
	month = ((int)file_modify_bytes[2]<<(8*(0)));
	day = ((int)file_modify_bytes[3]<<(8*(0)));
	hour = ((int)file_modify_bytes[4]<<(8*(0)));
	minute = ((int)file_modify_bytes[5]<<(8*(0)));
	second = ((int)file_modify_bytes[6]<<(8*(0)));

	printf(" %4d/%02d/%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
}


void listFiles(char * mmap, int offset, int length, int numRootblock){
	int i,j,k = 0;
	char *root_entry = (char *)malloc(sizeof(char) * length);     
	char *file_name_bytes = (char *)malloc(sizeof(char) * 31);
	char *file_modify_bytes = (char *)malloc(sizeof(char) * 7);
	unsigned char *file_size_bytes = (unsigned char *)malloc(sizeof(unsigned char) * FILESIZE);  
	int file_size = 0;
	for (i=0; i < numRootblock; i ++){	
		for (j = 0; j < 8; j ++){ 
			root_entry = memcpy(root_entry, mmap+offset+512*i+length*j, length);
			if ((root_entry[0] & 0x02) == 0x02){
				if ((root_entry[0] & 0x02) == 0x02){
					printf("F ");
				} else {
					printf("D ");
				}
				file_size_bytes = memcpy(file_size_bytes, root_entry + 9, 4);  
				file_size = 0;
				for(k=0; k < FILESIZE; k++){
					file_size += ((int)file_size_bytes[k]<<(8*(-k + FILESIZE - 1)));
				} //for
				printf("%10d ", file_size);
				file_name_bytes = memcpy(file_name_bytes, root_entry + 27, 31);
				printf("%30s ", file_name_bytes);
				file_modify_bytes = memcpy(file_modify_bytes, root_entry + 20, 7);
				print_time(file_modify_bytes);
				printf("\n");
			} //if
		} //for
	} //for
	free(root_entry);
	free(file_name_bytes);
	free(file_size_bytes);
}


/*

*/

#if defined(PART2)
void disklist(char** argv){
	struct SuperBlock_Struct start;
	int file;
	struct stat sf;
	char *mp;
	if((file = open(argv[1], O_RDONLY))){
		fstat(file, &sf);
		mp = mmap(NULL, sf.st_size, PROT_READ, MAP_SHARED, file, 0);
		start.block_size = get_super_info(mp, 8, 2);
		start.block_count = get_super_info(mp, 10, 4);
		start.fat_start = get_super_info(mp, 14, 4);
		start.fat_blocks = get_super_info(mp, 18, 4);
		start.root_start = get_super_info(mp, 22, 4);
		start.root_blocks = get_super_info(mp, 26, 4);
	} else {
		printf("Failed to open the file\n");
	}

	listFiles(mp, start.root_start*start.block_size, 64, start.root_blocks);

}
#endif

#if defined(PART3)
void diskget(char** argv){
	if(argv[3] != NULL){
		char* dir = strtok(argv[2], '/');
		int count = 0;
		while(dir != NULL){
			count++;
			dir = strtok(NULL, '/');
		} //count number of directories
		char * dirs[count];
		parse_directory(argv[2], *dirs);
		printf("%s", dirs);
	} else {
		char * dirs = '/';
		printf("%c", dirs);
	}
	



}
#endif

/*
MAIN FUNCTION

Defines all the parts in the assignment
*/

int main(int argc, char** argv){
  	#if defined(PART1)
		diskinfo(argv);
	#elif defined(PART2)
		disklist(argv);
	#elif defined(PART3)
		diskget(argv);
	#elif defined(PART4)
		printf("Part 4: diskput\n");
	#else
	#	error "PART[1234] must be defined"
	#endif
	return 0;
}