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

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

void print_data(Image image){
  // print type of image
  printf("P3\n");
  // print width height and color of image
  printf("%u %u\n255\n", image.width, image.height);
    // print pixels of image
  for (unsigned int counter_x = 0; counter_x < image.height; ++counter_x) {
    for (unsigned int counter_y = 0; counter_y < image.width; ++counter_y) {
      printf("%hu %hu %hu ", image.pixel[counter_x][counter_y][0],
                             image.pixel[counter_x][counter_y][1],
                             image.pixel[counter_x][counter_y][2]);
    }
    printf("\n");
  }
}

Image gray_scale(Image img) {
    for (unsigned int counter_x = 0; counter_x < img.height; ++counter_x) {
        for (unsigned int counter_y = 0; counter_y < img.width; ++counter_y) {
            int media = img.pixel[counter_x][counter_y][0] +
                        img.pixel[counter_x][counter_y][1] +
                        img.pixel[counter_x][counter_y][2];
            media /= 3;
            img.pixel[counter_x][counter_y][0] = media;
            img.pixel[counter_x][counter_y][1] = media;
            img.pixel[counter_x][counter_y][2] = media;
        }
    }

    return img;
}

Image blur(unsigned int height, unsigned short int pixel[512][512][3], int size, unsigned int width) {
    for (unsigned int counter_x = 0; counter_x < height; ++counter_x) {
        for (unsigned int counter_y = 0; counter_y < width; ++counter_y) {
            Pixel media = {0, 0, 0};

            int min_h = (height - 1 > counter_x + size/2) ? counter_x + size/2 : height - 1;
            int min_w = (width - 1 > counter_y + size/2) ? counter_y + size/2 : width - 1;
            for(int x = (0 > counter_x - size/2 ? 0 : counter_x - size/2); x <= min_h; ++x) {
                for(int y = (0 > counter_y - size/2 ? 0 : counter_y - size/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            media.red /= size * size;
            media.green /= size * size;
            media.blue /= size * size;

            pixel[counter_x][counter_y][0] = media.red;
            pixel[counter_x][counter_y][1] = media.green;
            pixel[counter_x][counter_y][2] = media.blue;
        }
    }
}

Image rotate90right(Image img) {
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
    for (unsigned int counter_x = 0; counter_x < height; ++counter_x) {
        for (unsigned int counter_y = 0; counter_y < width; ++counter_y) {
            pixel[counter_x][counter_y][0] = 255 - pixel[counter_x][counter_y][0];
            pixel[counter_x][counter_y][1] = 255 - pixel[counter_x][counter_y][1];
            pixel[counter_x][counter_y][2] = 255 - pixel[counter_x][counter_y][2];
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

Image mirror_image(Image image){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int w = image.width, h = image.height;

  if (horizontal == 1) w /= 2;
  else h /= 2;

  for (int i2 = 0; i2 < h; ++i2) {
      for (int j = 0; j < w; ++j) {
          int x = i2, y = j;

          if (horizontal == 1) y = image.width - 1 - j;
          else x = image.height - 1 - i2;

          Pixel aux1;
          aux1.red = image.pixel[i2][j][0];
          aux1.green = image.pixel[i2][j][1];
          aux1.blue = image.pixel[i2][j][2];

          image.pixel[i2][j][0] = image.pixel[x][y][0];
          image.pixel[i2][j][1] = image.pixel[x][y][1];
          image.pixel[i2][j][2] = image.pixel[x][y][2];

          image.pixel[x][y][0] = aux1.red;
          image.pixel[x][y][1] = aux1.green;
          image.pixel[x][y][2] = aux1.blue;
      }
  }
  return image;
}

Image sepia_filter(Image image){
  for (unsigned int x = 0; x < image.height; ++x) {
      for (unsigned int j = 0; j < image.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = image.pixel[x][j][0];
          pixel[1] = image.pixel[x][j][1];
          pixel[2] = image.pixel[x][j][2];

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int smallest_r = (255 >  p) ? p : 255;
          image.pixel[x][j][0] = smallest_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          smallest_r = (255 >  p) ? p : 255;
          image.pixel[x][j][1] = smallest_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          smallest_r = (255 >  p) ? p : 255;
          image.pixel[x][j][2] = smallest_r;
      }
  }
  return image;
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
              img = sepia_filter(img);
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
                    img = rotate90right(img);
                }
                break;
            }
            case 5: { // Espelhamento
                img = mirror_image(img);
                break;
            }
            case 6: { // Inversao de Cores
                invert_colors(img.pixel, img.width, img.height);
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
    print_data(img);

    return 0;
}
