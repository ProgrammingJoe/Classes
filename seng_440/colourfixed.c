#include<stdio.h>
#include<math.h>
#include <strings.h>
#include <stdlib.h>


#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

#pragma pack(pop)

int scale[] = {16, 128, 128};
// 3.13 Representation
float RGBtoYCCMatrix[] = {2105, 4128, 802, 33980, 35151, 3596, 3596, 35782, 33349};
//float YCCtoRGBMatrix[] = {1.164, 0, 1.596, 1.164, -0.391, -.0813, 1.164, 2.017, 0};

BITMAPINFOHEADER bitmapInfoHeader;
unsigned char *bitmapData;

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

float * matrixMult(float A[9], int B[3])
{
    static float f_product[] = {0,0,0};
    for(int x = 0; x < 3; x++)
    {
        f_product[0] = f_product[0] + A[x] * B[x];
        f_product[1] = f_product[1] + A[x+3] * B[x];
        f_product[2] = f_product[2] + A[x+6] * B[x];
    }

    return f_product;
}

int * RGBtoYCC(int RGB[3])
{
    float * f_product = matrixMult(RGBtoYCCMatrix, RGB);
    static int product[] = {0,0,0};

    for(int x = 0; x < 3; x++)
    {
        product[x] = round(f_product[x]) + scale[x];
    }
    return product;
}

/*int * YCCtoRGB(int YCC[3])
{
    for(int x = 0; x < 3; x++)
    {
        YCC[x] = YCC[x] - scale[x];
    }
    float * f_product = matrixMult(YCCtoRGBMatrix, YCC);
    static int product[] = {0,0,0};
    for(int x = 0; x < 3; x++)
    {
        product[x] = round(f_product[x]);
    }
    return product;
}*/

void printArray(int *array, int size)
{
    for(int x = 0; x < size; x++)
    {
        printf("%d\n", array[x]);
    }
}

void main()
{
    bitmapData = LoadBitmapFile("mountain.bmp",&bitmapInfoHeader);

    printf("Enter Colour Values in format X, X, X\n");
    fflush(stdout);
    int Colour[3];
    scanf("%d, %d, %d", Colour, Colour+1, Colour+2);
    fflush(stdout);
    
    printf("To convert RGB to YCbCr enter '1', for the reverse enter '2': ");
    int choice[1];
    scanf("%d", choice);
    fflush(stdout);
    if(choice[0] > 1)
        printArray(YCCtoRGB(Colour), 3);
    else
        printArray(RGBtoYCC(Colour), 3);
}
