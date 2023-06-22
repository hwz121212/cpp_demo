#include <windows.h>
#include <iostream>



int main (int argc, char **argv)// argc数组指针的长度，argv为指向字符串的数组指针，即每一个下标对应一个字符串数组
{
    int arg = 0, printusage = 1;
    //Keypoint k1, k2, k;
    FILE *in1 = 0, *in2 = 0, *out = 0;
    int  count = 0;

//假设命令行为：match -k1 standardImage.key -k2 unkownImage.key -out match.res，则

//argv[0]="match "

//argv[1]="-k1"

//argv[2]="standardImage.key"

//argv[3]="-k2"

//argv[4]="unkownImage.key"

//argv[5]="-out"

//argv[6]="match.res "

    /* Parse command line arguments.  */
    while (++arg < argc) {
      if (! strcmp(argv[arg], "-keys1") ||
          ! strcmp(argv[arg], "-k1")) {
        in1 = fopen(argv[arg+1], "r");
        arg++;
      }
      else if (! strcmp(argv[arg], "-keys2") ||
            !strcmp(argv[arg], "-k2")) {
        in2 = fopen(argv[arg+1], "r");
        arg++;
      }
      else if (! strcmp(argv[arg], "-out")) {
        out = fopen(argv[arg+1], "w");
        arg++;
      }
      else {
        if (argc>1)
          fprintf(stderr, "Invalid command line argument: %s\n", argv[arg]);
        printusage=2;
      }
    }


    if (in1 && in2 && printusage==1)
        printusage=0;



    if (printusage) {
      fprintf(stderr,
        "use: %s -keys1 [keyfile] -keys2 [keyfile] {-out [outfile]}\n",
        argv[0]);
      exit(1);
    }
    return 0;
}