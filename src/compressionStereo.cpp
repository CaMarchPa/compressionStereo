/***
 * Compression d'images stereo
 *
 * File : compressionStereo.cpp
 *
 * Description :
 *
 * TP6
 *
 * last edition : 27-11-16
 **/

 #include <iostream>
 #include <sstream>
 #include <stdio.h>
 #include <math.h>

 #include "ImageBase.h"


 using namespace std;

 int main (int argc, char ** argv) {

    char name_G[250], name_D[250];

    int height = 0, width = 0;


    ImageBase imIn_G, imIn_D;

    if (argc < 3) {
        cout << "usage : Image_Gauche.ppm Image_Droite.ppm" << endl;
        return EXIT_FAILURE;
    }

    /**---------INIT-----------**/
    sscanf(argv[1], "%s", name_G);
    sscanf(argv[2], "%s", name_D);

    imIn_G.load(name_G);
    imIn_D.load(name_D);

    height = imIn_G.getHeight();
    width = imIn_G.getWidth();

    /**--------YCbCr--------**/
        //Gauche
    ImageBase imgG_Y(width, height, true);
    ImageBase imgG_Cb(width, height, true);
    ImageBase imgG_Cr(width, height, true);
        //Droite
    ImageBase imgD_Y(width, height, true);
    ImageBase imgD_Cb(width, height, true);
    ImageBase imgD_Cr(width, height, true);



    //convertion --> YCbCr
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            //                                R                          G                            B
            imgG_Y[h][w]  =  (0.2990 * imIn_G[h*3][w*3]) + (0.587 * imIn_G[h*3][w*3+1]) + (0.114 * imIn_G[h*3][w*3+2]);
            imgG_Cb[h][w] =  (-0.1687 * imIn_G[h*3][w*3]) + (-0.3313 * imIn_G[h*3][w*3+1]) + (0.5 * imIn_G[h*3][w*3+2]) + 128;
            imgG_Cr[h][w] =  (0.50000 * imIn_G[h*3][w*3]) + (-0.4187 * imIn_G[h*3][w*3+1]) + (-0.0813 * imIn_G[h*3][w*3+2]) + 128;


        }
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            //                                R                          G                            B
            imgD_Y[h][w]  =  (0.2990 * imIn_D[h*3][w*3]) + (0.587 * imIn_D[h*3][w*3+1]) + (0.114 * imIn_D[h*3][w*3+2]);
            imgD_Cb[h][w] =  (-0.1687 * imIn_D[h*3][w*3]) + (-0.3313 * imIn_D[h*3][w*3+1]) + (0.5 * imIn_D[h*3][w*3+2]) + 128;
            imgD_Cr[h][w] =  (0.50000 * imIn_D[h*3][w*3]) + (-0.4187 * imIn_D[h*3][w*3+1]) + (-0.0813 * imIn_D[h*3][w*3+2]) + 128;


        }
    }

    char yG[20] = "imageG_Y.pgm", CbG[20] = "imageG_Cb.pgm", CrG[20] = "imageG_Cr.pgm";
    char yD[20] = "imageD_Y.pgm", CbD[20] = "imageD_Cb.pgm", CrD[20] = "imageD_Cr.pgm";

    imgG_Y.save(yG);        imgG_Cb.save(CbG);      imgG_Cr.save(CrG);

    imgG_Y.save(yD);        imgG_Cb.save(CbD);      imgG_Cr.save(CrD);

    //
    float matrixOfDiff[height][width]; // Stocker la différence entre l'image de Gauche et celle de Droite
    float matrixOfAVG[height][width];  // Stocker la moyenne entre -------/ /--------------/ /------------

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrixOfDiff[i][j] = imgG_Y[i][j] - imgD_Y[i][j];
            matrixOfAVG[i][j] = (1/2) * ( imgG_Y[i][j] + imgD_Y[i][j] );
        }
    }

    //construction de l'image droite en fonction de l'image gauche
    ImageBase imgDByDiff(width, height, true);
    ImageBase imgDByAvg(width, height, true);
    char diff[50] = "imgDByDifference.pgm";
    char avg[50] = "imgDByaverage.pgm";

    for (int i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            imgDByDiff[i][j] = imgG_Y[i][j] - matrixOfDiff[i][j];
            imgDByAvg[i][j] = 2*matrixOfAVG[i][j] - imgG_Y[i][j];
        }
    }
    //Save
    imgDByDiff.save(diff);
    imgDByAvg.save(avg);

    return EXIT_SUCCESS;
   }
