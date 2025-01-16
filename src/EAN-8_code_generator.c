#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(){
        printf("BARCODE GENERATOR\n");
        printf("\tGenerates a PBM file based on the input.\n");
        printf("Usage:\n");
        printf("\t/EAN-8_code_generator <option> ... <identifier>\n");
        printf("Options:\n");
        printf("\t-m <pixels>\n");
        printf("\t\tlet the user define the margin according to the integral input in <pixels>\n");
        printf("\t\twithout -m, the margin will be 4px\n");
        printf("\t-a <pixels>\n");
        printf("\t\tlet the user define the area for the code according to the integral input in <pixels>\n");
        printf("\t\twithout -a, the area will be 3px\n");
        printf("\t-h <pixels>\n");
        printf("\t\tlet the user define the height for the barcode according to the integral input in <pixels>\n");
        printf("\t\twithout -h, the height will be 50px\n");
        printf("\t-n <file_name>\n");
        printf("\t\tlet the user define the name for the output file according to the input in <file_name>\n");
        printf("\t\twithout -n, the name will be the identifier\n");
}

bool check_identifier(char *identifier) {
        if (strlen(identifier) != 8) {
                fprintf(stderr, "identifier must be 8 characters long\n");
                return false;
        }
        return true;

}

typedef struct {
        int width;
        int height;
        int **pixels;
} PBMImage;

typedef struct {
        int margin;
        int area;
        int height;
        int identifier[8];
        char *name;
} INPUTInfo;

int main(int argc, char *argv[]) {
        // 1. Receber um código de 8 dígitos e verificar se ele é válido;
        // 2. Se for válido, converter o código de 8 dígitos para código EAN-8;
        // 3. A partir do código EAN-8, criar uma imagem PBM



        //Ao chamar essa função sem nenhum argumento, exibe o uso no terminal
        if (argc == 1 || argc > 10) {
                usage();
                return 1;
        }
        INPUTInfo input = {4, 3, 50, -1};
        int opt;
        //Lida com os argumentos parseados
        //OBS: preciso trocar o atoi por strtol e considerar reportar erros

        while ((opt = getopt(argc, argv, ":m:a:h:n:")) != -1) {
                switch(opt){
                        case 'h':
                                printf("height = %d\n", atoi(optarg));
                                if(atoi(optarg) == 0) input.height = 50; else input.height = atoi(optarg);
                                break;
                        case 'm':
                                printf("margin = %d\n", atoi(optarg));
                                if(atoi(optarg) == 0) input.margin = 4; else input.margin = atoi(optarg);
                        break;
                        case 'a':
                                printf("area = %d\n", atoi(optarg));
                                if(atoi(optarg) == 0) input.area = 3; else input.area = atoi(optarg);
                        break;
                        case 'n':
                                printf("name = %s\n", optarg);
                                if(optarg == 0) input.name = argv[-1]; else input.name = optarg;
                                break;
                        case ':':
                                fprintf(stderr, "option needs a value");
                                return 1;
                        break;
                        case '?':
                                fprintf(stderr,"unknown option: %c\n", optopt);
                                return 1;
                        break;
                        default:
                                fprintf(stderr, "unexpected error");
                                return 1;
                }
        }
        //Verifica se o código de barra está sendo parseado
        if (optind < argc) {
                for(int i = optind; i < argc; i++) {
                        int num = atoi(argv[i]);
                        if (num > 0) {
                                if (input.identifier[0] == -1) {
                                        char *identifier = argv[optind];
                                        input.identifier[0] = atoi(identifier[4]);
                                        if (check_identifier(argv[i])) strcpy(input.identifier, argv[i]); else fprintf(stderr, "invalid identifier.\n");
                                        printf("identifier = %d\n", input.identifier);
                                } else {
                                        fprintf(stderr, "more than one identifier\n");
                                        return 1;
                                }
                        }
                        else {
                                fprintf(stderr, "invalid identifier");
                                return 1;
                        }
                }
        } else {
                fprintf(stderr, "Error: Missing barcode identifier.\n");
                return 1;
        }

        printf("Margin is %d\n", input.margin);
        printf("Area is %d\n", input.area);
        printf("Height is %d\n", input.height);
        printf("Name is %s\n", input.name);
        return 0;
}
