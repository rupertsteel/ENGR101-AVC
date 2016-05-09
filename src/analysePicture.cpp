#include "libE101.h"

void analysePicture(){
    take_picture();
    int sum = 0;
    int whiteness;
    for(int i = 0, i<320, i++){
        whiteness = get_pixel(i,120,3);
        if(whiteness>127){
            sum = sum + (i-160);
        }
    }
}
