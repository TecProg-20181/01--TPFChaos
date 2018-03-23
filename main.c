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

Image read_image_data(){
  Image img;
  char p3[4];
  int max_color;

  scanf("%s", p3);
  scanf("%u %u\n", &img.width, &img.height);
  scanf("%d\n",&max_color);

  for (unsigned int counter_x = 0; counter_x < img.height; ++counter_x) {
      for (unsigned int counter_y = 0; counter_y < img.width; ++counter_y) {
          scanf("%hu %hu %hu", &img.pixel[counter_x][counter_y][0],
                               &img.pixel[counter_x][counter_y][1],
                               &img.pixel[counter_x][counter_y][2]);
      }
  }
      return img;
}
Image gray_scale(Image image) {
    for (unsigned int counter_x = 0; counter_x < image.height; ++counter_x) {
        for (unsigned int counter_y = 0; counter_y < image.width; ++counter_y) {
            int media = image.pixel[counter_x][counter_y][0] +
                        image.pixel[counter_x][counter_y][1] +
                        image.pixel[counter_x][counter_y][2];
            media /= 3;
            image.pixel[counter_x][counter_y][0] = media;
            image.pixel[counter_x][counter_y][1] = media;
            image.pixel[counter_x][counter_y][2] = media;
        }
    }

    return image;
}

Image blur(unsigned int height, unsigned short int pixel[512][512][3], unsigned int width) {
    int size = 0;
    scanf("%d", &size);
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

Image rotate90right(Image image) {
    Image rotated_image;
    int number_of_rotations = 0;
    scanf("%d", &number_of_rotations);
    number_of_rotations %= 4;
    for (int j1 = 0; j1 < number_of_rotations; ++j1) {

      rotated_image.width = image.height;
      rotated_image.height = image.width;

      for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
          for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
              rotated_image.pixel[i][j][0] = image.pixel[x][y][0];
              rotated_image.pixel[i][j][1] = image.pixel[x][y][1];
              rotated_image.pixel[i][j][2] = image.pixel[x][y][2];
          }
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

Image cut_image(Image image) {
    Image cut;
    int cut_x, cut_y;
    int width, height;

    scanf("%d %d", &cut_y, &cut_x);
    scanf("%d %d", &width, &height);

    cut.width = width;
    cut.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cut.pixel[i][j][0] = image.pixel[i + cut_x][j + cut_y][0];
            cut.pixel[i][j][1] = image.pixel[i + cut_x][j + cut_y][1];
            cut.pixel[i][j][2] = image.pixel[i + cut_x][j + cut_y][2];
        }
    }

    return cut;
}

Image mirror_image(Image image){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = image.width, height = image.height;

  if (horizontal == 1) width /= 2;
  else height /= 2;

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
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
    img = read_image_data();

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
                blur(img.height, img.pixel, img.width);
                break;
            }
            case 4: { // Rotacao
                img = rotate90right(img);
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
                img = cut_image(img);
                break;
            }
        }

    }
    print_data(img);

    return 0;
}
