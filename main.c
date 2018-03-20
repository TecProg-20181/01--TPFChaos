#include <stdio.h>

typedef struct _pixel {
    unsigned short int red, green,blue ;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width, height;
} Image;


Image gray_scale(Image img) {
    for (unsigned int image_height = 0; image_height < img.height; ++image_height) {
        for (unsigned int image_width = 0; image_width < img.width; ++image_width) {
            int media = img.pixel[image_height][image_width][0] +
                        img.pixel[image_height][image_width][1] +
                        img.pixel[image_height][image_width][2];
            media /= 3;
            img.pixel[image_height][image_width][0] = media;
            img.pixel[image_height][image_width][1] = media;
            img.pixel[image_height][image_width][2] = media;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int image_height = 0; image_height < height; ++image_height) {
        for (unsigned int image_width = 0; image_width < width; ++image_width) {
            Pixel media = {0, 0, 0};

            int min_h = (height - 1 > image_height + T/2) ? image_height + T/2 : height - 1;
            int min_w = (width - 1 > image_width + T/2) ? image_width + T/2 : width - 1;
            for(int x = (0 > image_height - T/2 ? 0 : image_height - T/2); x <= min_h; ++x) {
                for(int y = (0 > image_width - T/2 ? 0 : image_width - T/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            pixel[image_height][image_width][0] = media.red;
            pixel[image_height][image_width][1] = media.green;
            pixel[image_height][image_width][2] = media.blue;
        }
    }
}

Image rotate90direita(Image img) {
    Image rotated_image;

    rotated_image.width = img.height;
    rotated_image.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
        for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated_image.pixel[i][j][0] = img.pixel[x][y][0];
            rotated_image.pixel[i][j][1] = img.pixel[x][y][1];
            rotated_image.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotated_image;
}

void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int image_height = 0; image_height < height; ++image_height) {
        for (unsigned int image_width = 0; image_width < width; ++image_width) {
            pixel[image_height][image_width][0] = 255 - pixel[image_height][image_width][0];
            pixel[image_height][image_width][1] = 255 - pixel[image_height][image_width][1];
            pixel[image_height][image_width][2] = 255 - pixel[image_height][image_width][2];
        }
    }
}

Image cut_image(Image img, int x, int y, int w, int h) {
    Image cut;

    cut.width = w;
    cut.height = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cut.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cut.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cut.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cut;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                img = gray_scale(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.height; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x][j][0];
                        pixel[1] = img.pixel[x][j][1];
                        pixel[2] = img.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][0] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][1] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotate90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.width, h = img.height;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i2;

                        Pixel aux1;
                        aux1.red = img.pixel[i2][j][0];
                        aux1.green = img.pixel[i2][j][1];
                        aux1.blue = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x][y][0];
                        img.pixel[i2][j][1] = img.pixel[x][y][1];
                        img.pixel[i2][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.red;
                        img.pixel[x][y][1] = aux1.green;
                        img.pixel[x][y][2] = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                invert_colours(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = cut_image(img, x, y, w, h);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int image_height = 0; image_height < img.height; ++image_height) {
        for (unsigned int image_width = 0; image_width < img.width; ++image_width) {
            printf("%hu %hu %hu ", img.pixel[image_height][image_width][0],
                                   img.pixel[image_height][image_width][1],
                                   img.pixel[image_height][image_width][2]);

        }
        printf("\n");
    }
    return 0;
}
