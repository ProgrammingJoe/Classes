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
#define MAXLENGTH 25

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
 
typedef struct FolderLocation_Struct {
	uint32_t offset;
	uint32_t numblocks;
} FolderLocation_t;

typedef struct FolderFileLocation_Struct {
	uint32_t offset;
	uint32_t numblocks;
	char * filename;
} FolderFileLocation_t;


/*

DECLARATIONS ABOVE

*/

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

FolderLocation_t find_folder_part2(char * folder, char * mp, int offset, int length, int numblock, int blocksize){
	int i,j = 0;
	char *root_entry = (char *)malloc(sizeof(char) * length);
	char *file_name_bytes = (char *)malloc(sizeof(char) * 31);
	for (i=0; i < numblock; i ++){	
		for (j = 0; j < 8; j ++){ 
			root_entry = memcpy(root_entry, mp+offset+512*i+length*j, length);
			if ((root_entry[0] & 0x7) == 0x5 || (root_entry[0] & 0x7) == 0x3){	
				file_name_bytes = memcpy(file_name_bytes, root_entry + 27, 31);
				if((strcmp(file_name_bytes, folder) == 0) && ((root_entry[0] & 0x7) == 0x5)){
					folder = strtok(NULL, "/");
					if(folder != NULL){
						int start = get_super_info(mp, offset+512*i+length*j+1, 4);
						int numblock = get_super_info(mp, offset+512*i+length*j+5, 4);
						return find_folder_part2(folder, mp, start, 64, numblock, blocksize);
					} else {
						struct FolderLocation_Struct fold;
						fold.offset = get_super_info(mp, offset+512*i+length*j+1, 4);
						fold.numblocks = get_super_info(mp, offset+512*i+length*j+5, 4);
						return fold;
					}
				} //if
			} //if
		} //for
	} //for
	printf("Folder not Found.\n");
	exit(1);
	free(root_entry);
	free(file_name_bytes);
}

FolderFileLocation_t find_folder_part3(char * folders[], char * mp, int offset, int length, int numblock, int blocksize, int pre, int cur, int fut){
	int i,j = 0;
	char *root_entry = (char *)malloc(sizeof(char) * length);
	char *file_name_bytes = (char *)malloc(sizeof(char) * 31);
	for (i=0; i < numblock; i ++){	
		for (j = 0; j < 8; j ++){ 
			root_entry = memcpy(root_entry, mp+offset+512*i+length*j, length);
			if ((root_entry[0] & 0x7) == 0x5 || (root_entry[0] & 0x7) == 0x3){
				file_name_bytes = memcpy(file_name_bytes, root_entry + 27, 31);
				if((strcmp(file_name_bytes, folders[cur]) == 0) && ((root_entry[0] & 0x7) == 0x5)){
					if(strcmp(folders[pre], folders[cur]) == 0){
						cur++;
						fut++;
					} else {
						pre++;
						cur++;
						fut++;
					}
					if(folders[fut] != NULL){
						int start = get_super_info(mp, offset+512*i+length*j+1, 4);
						int numblock = get_super_info(mp, offset+512*i+length*j+5, 4);
						return find_folder_part3(folders, mp, start, 64, numblock, blocksize, pre, cur, fut);
					} else {
						struct FolderFileLocation_Struct fold;
						fold.offset = get_super_info(mp, offset+512*i+length*j+1, 4);
						fold.numblocks = get_super_info(mp, offset+512*i+length*j+5, 4);
						fold.filename = folders[cur];
						return fold;
					}
				} //if
			} //if
		} //for
	} //for

	printf("Folder not Found.\n");
	exit(1);
	free(root_entry);
	free(file_name_bytes);
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
			if ((root_entry[0] & 0x7) == 0x5 || (root_entry[0] & 0x7) == 0x3){
				if ((root_entry[0] & 0x7) == 0x3){
					printf("F ");
				} else if((root_entry[0] & 0x7) == 0x5){
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
void append_file(char * mp, FILE * file, int start, int blocksize){
	int offset = start*blocksize;
	int filesize = get_super_info(mp, 600 , 4);
	char * the_file = (char *)malloc(sizeof(char) * filesize);
	the_file = memcpy(the_file, mp+offset, filesize);
	fwrite(the_file, filesize, 1, file);
	if((get_super_info(mp, offset/blocksize, 4)) == 0 || (get_super_info(mp, offset/blocksize, 4)) == 1 ){
		exit(1);
	} else {
		append_file(mp, file, offset, blocksize);
	}
}
*/

void find_filename_part3(char * mp, int offset, int length, int numRootblock, char * name, int blocksize, char * filename, int fat_start){
	int i,j = 0;
	char *root_entry = (char *)malloc(sizeof(char) * length);
	char *file_name_bytes = (char *)malloc(sizeof(char) * 31);
	FILE * file;
	for (i=0; i < numRootblock; i ++){	
		for (j = 0; j < 8; j ++){ 
			root_entry = memcpy(root_entry, mp+offset+512*i+length*j, length);
			if ((root_entry[0] & 0x02) == 0x02){
				file_name_bytes = memcpy(file_name_bytes, root_entry + 27, 31);
				if(strcmp(file_name_bytes, filename) == 0){
					int start = get_super_info(mp, offset+512*i+length*j+1, 4);
					int filesize = get_super_info(mp, offset+512*i+length*j+9, 4);
					char * the_file = (char *)malloc(sizeof(char) * filesize);
					the_file = memcpy(the_file, mp+start*blocksize, filesize);
					file = fopen(name, "wb");
					fwrite(the_file, filesize, 1, file);
					//if((get_super_info(mp, start, 4)) == 0 || (get_super_info(mp, start, 4)) == 1){
					//	printf("I shouldnt go here for mkfile.cc\n");
					exit(1);
					//} else {
					//	append_file(mp, file, start, blocksize);
					//}
				}
			} //if
		} //for
	} //for
	printf("File not Found.\n");
	free(root_entry);
	free(file_name_bytes);
}

int find_space(char* mp, int blocksneeded, int fatstart, int fatblocks, int block_size){
	int freecount = 0;
	int x = 0;
	for(x = 0; x < fatblocks*block_size; x += 4){
		uint32_t value = 0;
		value = get_super_info(mp, fatstart*block_size + x, 4);
		if(freecount == blocksneeded){
			int startfree = (fatstart*block_size+x - (blocksneeded-1)*4);
			return startfree;
		} else if(value == 0){
			freecount += 1;
		} else {
			freecount = 0;
		}
	}
	return -1;
}

int get_filesize(char * mp, int offset, int length, int numRootblock, char * filename){
	int i,j = 0;
	char *root_entry = (char *)malloc(sizeof(char) * length);
	char *file_name_bytes = (char *)malloc(sizeof(char) * 31);
	for (i=0; i < numRootblock; i ++){	
		for (j = 0; j < 8; j ++){ 
			root_entry = memcpy(root_entry, mp+offset+512*i+length*j, length);
			if ((root_entry[0] & 0x02) == 0x02){
				file_name_bytes = memcpy(file_name_bytes, root_entry + 27, 31);
				printf("#%s#\n", file_name_bytes);
				if(strcmp(file_name_bytes, filename) == 0){
					int filesize = get_super_info(mp, offset+512*i+length*j+9, 4);
					return filesize;
				}
			} //if
		} //for
	} //for
	printf("File not Found.\n");
	exit(1);
	free(root_entry);
	free(file_name_bytes);
}

void copy_file(char* imgmp, char* filemp, int block, int fatfreestart, int blocksize){
	int x = 0;
	for(x = 0; x < block; x++){
		memcpy(imgmp+fatfreestart+x*blocksize, filemp+x*blocksize, blocksize);
	}
}


void update_fat(int fatfreestart, int blocks){
}

void add_directory_entries(int fatfreestart, int blocks){
}

/////////PART 1///////////////////////


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
	fat.free_blocks = 0;
	fat.reserved_blocks = 0;
	fat.allocated_blocks = 0;
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


////////////////////PART 2/////////////////


#if defined(PART2)
void disklist(char** argv){
	struct SuperBlock_Struct start;
	int file;
	struct stat sf;
	char *mp;
	char * path = argv[2];
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
		printf("Failed to open the file.\n");
	}

	char * token;
	token = strtok(path, "/");
	if(token != NULL){
		struct FolderLocation_Struct fold;
		fold = find_folder_part2(token, mp, start.root_start*start.block_size, 64, start.root_blocks, start.block_size);
		listFiles(mp, fold.offset*start.block_size, 64, fold.numblocks);
	} else {
		listFiles(mp, start.root_start*start.block_size, 64, start.root_blocks);
	}
}
#endif


//////////////////PART 3////////////////


#if defined(PART3)
void diskget(char** argv){
	char * name = argv[2];
	char * path = argv[3];

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
	
	char * folders[MAXLENGTH];
	int x = 0;
	for(x = 0; x< MAXLENGTH; x++){
		folders[x] = NULL;
	}
	char * token;
	int pre  = 0, cur = 0;
	int fut = 1;
	int i = 0;
	token = strtok(path, "/");
	while(token != NULL){
		folders[i] = malloc(strlen(token)*sizeof(char)+1);
		strcpy(folders[i++], token);
		token = strtok(NULL, "/");
	}
	
	if(folders[fut] != NULL){
		struct FolderFileLocation_Struct fold;
		fold = find_folder_part3(folders, mp, start.root_start*start.block_size, 64, start.root_blocks, start.block_size, pre, cur, fut);
		find_filename_part3(mp, fold.offset*start.block_size, 64, fold.numblocks, name, start.block_size, fold.filename, start.fat_start);
	} else {
		find_filename_part3(mp, start.root_start*start.block_size, 64, start.root_blocks, name, start.block_size, folders[cur], start.fat_start);
	}
}
#endif


////////////PART 4///////////////////


#if defined(PART4)
void diskput(char ** argv){
	struct SuperBlock_Struct start;
	int file;
	struct stat sf;
	char *mp;
	char* name = argv[2];
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
		printf("File not Found.\n");
	}

	struct SuperBlock_Struct start2;
	struct stat sf2;
	int file2;
	char* mp2;
	if((file2 = open(argv[2], O_RDONLY))){
		fstat(file2, &sf2);
		mp2 = mmap(NULL, sf.st_size, PROT_READ, MAP_SHARED, file, 0);
		start2.block_size = get_super_info(mp, 8, 2);
		start2.block_count = get_super_info(mp, 10, 4);
		start2.fat_start = get_super_info(mp, 14, 4);
		start2.fat_blocks = get_super_info(mp, 18, 4);
		start2.root_start = get_super_info(mp, 22, 4);
		start2.root_blocks = get_super_info(mp, 26, 4);
	} else {
		printf("File not Found.\n");
	}
	
	int filesize = get_filesize(mp2, start2.root_start, 64, start2.root_blocks, name);
	int blocks = filesize/start.block_size + 1;
	int fatfreestart = find_space(mp, blocks, start.fat_start, start.fat_blocks, start.block_size);
	if(fatfreestart == -1){
		printf("No free space in disk\n");	
	} else {
		copy_file(mp, mp2, blocks, fatfreestart, start.block_size);
		update_fat(fatfreestart, blocks);
		add_directory_entries(fatfreestart, blocks);
	}
}
#endif

/*
MAIN FUNCTION

Defines all the parts in the assignment
*/

int main(int argc, char** argv){
  	#if defined(PART1)
  		if(argc == 2){
			diskinfo(argv);
  		} else {
  			printf("Input should be of the form: ./diskinfo <file system>\n");
  		}
	#elif defined(PART2)
		if(argc == 3 || argc == 2){
			disklist(argv);
  		} else {
  			printf("Input should be of the form: ./disklist <file system> <sub directory>|BLANK\n");
  		}
	#elif defined(PART3)
		if(argc == 4 || argc == 3){
			diskget(argv);
  		} else {
  			printf("Input should be of the form: ./diskget <file system> <filename> <sub directory>|BLANK\n");
  		}
	#elif defined(PART4)
		if(argc == 4 || argc == 3){
			diskput(argv);
  		} else {
  			printf("Input should be of the form: ./diskput <file system> <filename> <sub directory>|BLANK\n");
  		}	
	#else
	#	error "PART[1234] must be defined"
	#endif
	return 0;
}