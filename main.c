#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Param        //parameters of the points
{
    float x;
    float y;
} Coord;

typedef struct line         //parameters of the lines
{
    float k;
    float b;
} Line_param;

void getParamSize(Coord* size);                        

void getParamHor(float* yhor);                    

void getParamSun(Coord* sun);               

void getParamColumn(Coord* column_vertex, Coord* column_bottom);         

void lineParam(Coord column_param, Coord sun_or_projection, Line_param* line_1_2);                        
char shadowParam(Line_param line_1, Line_param line_2, Coord column_bottom, float column_vertex_y, Coord size, Coord sun, float yhor, Coord* shadow);      



int main()
{
    Coord  size;                     // x0, y0 - size of the picture
    Coord  sun;                      //xs, ys - sun coordinates
    Coord  column_vertex;            //x1, y1 - coordinates of the column vertex
    Coord  column_bottom;            //x2, y2 - coordinates of the bottom of the column
    Coord  sun_projection;           //xh, yh - projection of the sun on the horizon line
    Coord  shadow;                   //xsh, ysh - coordinates of the farthest point of the shadow

    Line_param  line_1;              //upper line
    Line_param  line_2;              //lower line

    float yhor;                      //coordinates of the horizon line
    char flag;

    getParamSize(&size);                                            //parameters of the picture
    getParamHor(&yhor);                                     //parameters of the horizon line
    getParamSun(&sun);                                  //parameters of sun
    getParamColumn(&column_vertex, &column_bottom);           //parameters of the column

    sun_projection.x = sun.x;               //parameters of the sn projection
    sun_projection.y = yhor;

    lineParam(column_vertex, sun, &line_1);             //parameters of auxiliary lines for shadow construction
    lineParam(column_bottom, sun_projection, &line_2);

    flag = shadowParam(line_1, line_2, column_bottom, column_vertex.y, size, sun, yhor, &shadow);                //building a shadow
    switch (flag)
    {
        case 1:                                                                     
            printf("%.2f %.2f\n", shadow.x, shadow.y);
            break;

        case 2:                                                                       
            printf("NO\n");
            break;
    }

    return 0;
}

 void getParamSize(Coord* size)                                    //we get the dimensions of the image
{
    Coord size1;
    scanf("%f%f", &size1.x, &size1.y);
    size -> x = size1.x;
    size -> y = size1.y;
}

void getParamHor(float* yhor)                             //we get the parameters of the horizon
{
     float yhor1;
     scanf("%f", &yhor1);
     *yhor = yhor1;
}

void getParamSun(Coord* sun)                            //we get the parameters of the sun
{
    Coord sun1;
    scanf("%f%f", &sun1.x, &sun1.y);
    sun -> x = sun1.x;
    sun -> y = sun1.y;
}

void getParamColumn(Coord* column_vertex, Coord* column_bottom)                          //we get the parameters of the column
{
    Coord vertex;
    Coord bottom;
    scanf("%f%f", &vertex.x, &vertex.y);
    column_vertex -> x = vertex.x;
    column_vertex -> y = vertex.y;
    scanf("%f%f", &bottom.x, &bottom.y);
    column_bottom -> x = bottom.x;
    column_bottom -> y = bottom.y;
}

void lineParam(Coord column_param, Coord sun_or_projection, Line_param* line_1_2)                //the beginning of shadow construction
{
    float kk;
    line_1_2 -> k = ((column_param.y - sun_or_projection.y) / (column_param.x - sun_or_projection.x));
    kk = line_1_2 -> k;
    line_1_2 -> b = (column_param.y - kk * column_param.x);
}

char shadowParam(Line_param line_1, Line_param line_2, Coord column_bottom, float column_vertex_y, Coord size, Coord sun, float yhor, Coord* shadow)                //building a shadow
{
    float x, y;

    if (column_bottom.x == 0 || column_bottom.x == size.x || column_bottom.y == size.y)          
        return 2;

    if (column_bottom.y == yhor && sun.y > yhor)                                
        return 2;

    if (column_bottom.x == sun.x)               
    {
        if (sun.y >= column_bottom.y)
            return 2;
        else if (sun.y < column_bottom.y)
        {
            shadow -> x = column_bottom.x;
            shadow -> y = size.y;
            return 1;
        }
    }

    if (sun.y >= yhor)                               
        return 2;
    if (sun.y < yhor)                                        
    {
        if (sun.y > column_vertex_y)
        {
            float position_x1;
            if (sun.x > column_bottom.x)
                position_x1 = 0;
            if (sun.x < column_bottom.x)
                position_x1 = size.x;
            if ((line_2.k * position_x1 + line_2.b) <= size.y)
            {
                shadow -> x = position_x1;
                shadow -> y = (line_2.k * position_x1 + line_2.b);
                return 1;
            }
            else if ((line_2.k * position_x1 + line_2.b) > size.y)
            {
                shadow -> y = size.y;
                shadow -> x = (size.y - line_2.b) / line_2.k;
                return 1;
            }
        }
        x = (line_1.b - line_2.b) / (line_2.k - line_1.k);
        y = line_1.k * x + line_1.b;
        if (x <= size.x && x >= 0 && y <= size.y && y >= 0)
        {
            shadow -> x = x;
            shadow -> y = y;
            return 1;
        }
        if (fabs(line_1.k - line_2.k) < 0.00001)
        {
            float position_x2;
            if (sun.x > column_bottom.x)
                position_x2 = 0;
            if (sun.x < column_bottom.x)
                position_x2 = size.x;
            if ((line_2.k * position_x2 + line_2.b) <= size.y)
            {
                shadow -> x = position_x2;
                shadow -> y = line_2.k * position_x2 + line_2.b;
            }
            else if ((line_2.k * position_x2 + line_2.b) > size.y)
            {
                shadow -> y = size.y;
                shadow -> x = (size.y - line_2.b) / line_2.k;
            }
            return 1;
        }
        if (x > size.x || y > size.y )
        {
            float position_x3;
            if (sun.x > column_bottom.x)
                position_x3 = 0;
            if (sun.x < column_bottom.x)
                position_x3 = size.x;
            if ((line_2.k * position_x3 + line_2.b) <= size.y)
            {
                shadow -> x = position_x3;
                shadow -> y = (line_2.k * position_x3 + line_2.b);
                return 1;
            }
            else if ((line_2.k * position_x3 + line_2.b) > size.y)
            {
                shadow -> y = size.y;
                shadow -> x = (size.y - line_2.b) / line_2.k;
                return 1;
            }
        }
    }
}
