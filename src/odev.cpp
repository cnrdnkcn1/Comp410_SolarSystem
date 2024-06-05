/*
Sinan Ekmekcibasi - 87653
Caner Denkcan - 71671

*/
#include "../include/Angel.h"

#include <iostream>

typedef vec4 color4;
typedef vec4 point4;

// RGBA olors
color4 vertex_colors[8] = {
    color4(0.0, 1.0, 1.0, 1.0),  // cyan
    color4(1.0, 0.0, 0.0, 1.0), // red
    color4(1.0, 1.0, 0.0, 1.0), // yellow
    color4(0.0, 1.0, 0.0, 1.0), // green
    color4(0.0, 0.0, 1.0, 1.0), // blue
    color4(1.0, 0.0, 1.0, 1.0), // magenta
    color4(1.0, 1.0, 1.0, 1.0), // white
    color4(0.0, 0.0, 0.0, 1.0) // black

};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[11][NumAxes];
GLfloat  ThetaR[NumAxes] = {16,0,0};


int mode = 1;  // 0:cube, 1:sphere, 2:bunny
int dmode = 1; // drawing mode
int shadeMode = 1; // 0: Gouraud, 1: Phond
int componentMode = 0; // 0: all, 1: without specular,  2: without diffuse, 3: without ambient 
int lightMode = 0;
int materialMode = 0;  // 0: metalic, 1: plastic
int displayMode = 0; //0: wireframe, 1: shading, 2: texture
int objectMode = 1;  //0: basketball, 1: earth
int colorInt = 0;   //color

// Model-view and projection matrices uniform location
GLuint ModelView, Projection;

const GLfloat z_near = 0.1;
const GLfloat z_far = 6.0;

//only sphere and walls for shading
const GLuint NUM_OBJECTS = 11;

vec3 zoom = { 0,0,0 };

//textures
GLuint textures[11];


GLubyte image[2048][1024][3];
GLubyte image2[2048][1024][3];
GLubyte image3[2048][1024][3];
GLubyte image4[2048][1024][3];
GLubyte image5[2048][1024][3];
GLubyte image6[2048][1024][3];
GLubyte image7[2048][1024][3];
GLubyte image8[2048][1024][3];
GLubyte image9[2048][1024][3];
GLubyte image11[2048][1024][3];
GLubyte image10[512][512][3];



GLubyte(*images[10])[2048][1024][3] = { &image, &image2, &image3, &image4, &image5, &image6, &image7, &image8, &image9, &image11 };
GLubyte(*image10_ptr)[512][512][3] = &image10;


GLuint  TextureFlagLoc; // texture flag uniform location
bool textureFlag = true;


const GLfloat SCALE_FACTOR = 0.05;

const GLfloat scales[11] = { 0.25, 0.05 ,0.1 ,0.1 ,0.07, 0.15 ,0.12, 0.10, 0.12, 0.04 , 0.5};
const GLfloat thetas[11] = { 0.6, 0.8, 0.5, 0.6, 0.7, 0.45, 0.25, 0.15, 0.35, 0.5,-5 };
const vec3 distance[11] = {
    vec3(0.00, 0.0, 0.0),
    vec3(0.20, 0.0, 0.0),
    vec3(0.35, 0.0, 0.0),
    vec3(0.50, 0.0, 0.0),
    vec3(0.65, 0.0, 0.0),
    vec3(0.80, 0.0, 0.0),
    vec3(0.95, 0.0, 0.0),
    vec3(1.10, 0.0, 0.0),
    vec3(1.25, 0.0, 0.0),
    vec3(1.40, 0.0, 0.0),
    vec3(1.55, 0.0, 0.0)
};

const GLfloat FOV = 90.0;

// Wall boundaries
GLfloat leftWall = -3.0;
GLfloat rightWall = 3.0;
GLfloat bottomWall = -3.0;
GLfloat topWall = 3.0;
GLfloat backWall = -6.0;
GLfloat frontWall = -z_near - 2;

// Speed values for movement
const GLfloat HORIZONTAL_SPEED = 0.009;
const GLfloat VERTICAL_SPEED = 0.006;
const GLfloat Z_SPEED = 0.003;

// velocity and direction values
vec3 velo(HORIZONTAL_SPEED, VERTICAL_SPEED, Z_SPEED);
int direction[3] = { 1, 1, 1 };

const vec3 TOP_LEFT_FRONT_CORNER = vec3(0, 0, -1); //vec3(-0.75, 0.75, -2.0);

//point4 light_position(0.0, 0.0, -2.0, 1.0);
point4 light_position(TOP_LEFT_FRONT_CORNER.x, TOP_LEFT_FRONT_CORNER.y, TOP_LEFT_FRONT_CORNER.z, 1.0);
GLuint denemee;

// Storing objects and wall in the vertex array object (VAO)
GLuint vao[NUM_OBJECTS];

vec3 displacement = TOP_LEFT_FRONT_CORNER;
// We use namepsaces to put together functions and datas regarding each object.

namespace sphere
{
    GLuint buffer;

    const int numVertices = 50000;

    int k = 0;
    int l = 0;

    int colorInt = 0;

    vec4 points[numVertices];
    vec4 colors[numVertices];
    vec3 normals[numVertices];
    vec2 tex_coords[numVertices];

    point4 v[4] = { vec4(0.0, 0.0, 1.0, 1.0),
                   vec4(0.0, 0.942809, -0.333333, 1.0),
                   vec4(-0.816497, -0.471405, -0.333333, 1.0),
                   vec4(0.816497, -0.471405, -0.333333, 1.0) };

    // Generate a triangle with normals and texture coordinates
    void triangle(point4 a, point4 b, point4 c)
    {
        vec3 norm = normalize(vec3(a.x, a.y, a.z));
        points[k] = a; normals[k] = norm; tex_coords[k] = vec2(atan2(norm.z, norm.x) / (2 * M_PI) + 0.5, asin(norm.y) / M_PI + 0.5); k++;

        norm = normalize(vec3(b.x, b.y, b.z));
        points[k] = b; normals[k] = norm; tex_coords[k] = vec2(atan2(norm.z, norm.x) / (2 * M_PI) + 0.5, asin(norm.y) / M_PI + 0.5); k++;

        norm = normalize(vec3(c.x, c.y, c.z));
        points[k] = c; normals[k] = norm;  tex_coords[k] = vec2(atan2(norm.z, norm.x) / (2 * M_PI) + 0.5, asin(norm.y) / M_PI + 0.5); k++;

    }

    // Recursively subdivide a triangle
    void divide_triangle(point4 a, point4 b, point4 c, int n)
    {
        point4 v1, v2, v3;
        if (n > 0)
        {

            v1 = (a + b) / 2;
            v2 = (a + c) / 2;
            v3 = (b + c) / 2;

            float asd = pow(v1[0], 2);
            float asd2 = pow(v1[1], 2);
            float asd3 = pow(v1[2], 2);
            float x1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2));
            float x2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2) + pow(v2[2], 2));
            float x3 = sqrt(pow(v3[0], 2) + pow(v3[1], 2) + pow(v3[2], 2));

            v1[0] = v1[0] / x1;
            v1[1] = v1[1] / x1;
            v1[2] = v1[2] / x1;
            v2[0] = v2[0] / x2;
            v2[1] = v2[1] / x2;
            v2[2] = v2[2] / x2;
            v3[0] = v3[0] / x3;
            v3[1] = v3[1] / x3;
            v3[2] = v3[2] / x3;

            divide_triangle(a, v2, v1, n - 1);
            divide_triangle(c, v3, v2, n - 1);
            divide_triangle(b, v1, v3, n - 1);
            divide_triangle(v1, v2, v3, n - 1);
        }
        else
            triangle(a, b, c);
    }

    int a = 0;

    void colorize(int colorInt)
    {
        if (colorInt == 7)
        {
            colorInt = 0;
        }

        while (a < numVertices)
        {
            colors[a] = vertex_colors[colorInt];
            a++;
        }
        a = 0;
    }

    //Create a sphere
    void tetrahedron(int n)
    {
        divide_triangle(v[0], v[1], v[2], n);
        divide_triangle(v[3], v[2], v[1], n);
        divide_triangle(v[0], v[3], v[1], n);
        divide_triangle(v[0], v[2], v[3], n);
        colorize(colorInt);
    }



    // Assign colors to the sphere


} // namespace sphere



namespace wall
{
    GLuint buffer;
    const int numVertices = 30;

    vec4 points[numVertices];
    vec4 colors[numVertices];
    vec4 normals[numVertices];
    vec2 tex_coords[numVertices];

    point4 vertices[8] = {
        point4(-1.0, -1.0, 1.0, 1.0),
        point4(-1.0, 1.0, 1.0, 1.0),
        point4(1.0, 1.0, 1.0, 1.0),
        point4(1.0, -1.0, 1.0, 1.0),
        point4(-1.0, -1.0, -1.0, 1.0),
        point4(-1.0, 1.0, -1.0, 1.0),
        point4(1.0, 1.0, -1.0, 1.0),
        point4(1.0, -1.0, -1.0, 1.0),
    };

    int Index = 0;

    //Creating a face of cube and normals
    void quad(int a, int b, int c, int d, int color)
    {
        vec4 u = vertices[b] - vertices[a];
        vec4 v = vertices[c] - vertices[b];

        vec4 normal = normalize(cross(u, v));


        colors[Index] = vertex_colors[color];
        points[Index] = vertices[a];
        normals[Index] = normal;
        tex_coords[Index] = vec2(0.0, 0.0);
        Index++;
        colors[Index] = vertex_colors[color];
        points[Index] = vertices[b];
        normals[Index] = normal;
        tex_coords[Index] = vec2(0.0, 1.0);
        Index++;
        colors[Index] = vertex_colors[color];
        points[Index] = vertices[c];
        normals[Index] = normal;
        tex_coords[Index] = vec2(1.0, 1.0);
        Index++;
        colors[Index] = vertex_colors[color];
        points[Index] = vertices[a];
        normals[Index] = normal;
        tex_coords[Index] = vec2(0.0, 0.0);
        Index++;
        colors[Index] = vertex_colors[color];
        points[Index] = vertices[c];
        normals[Index] = normal;
        tex_coords[Index] = vec2(1.0, 1.0);
        Index++;
        colors[Index] = vertex_colors[color];
        points[Index] = vertices[d];
        normals[Index] = normal;
        tex_coords[Index] = vec2(1.0, 0.0);
        Index++;
    }

    void update()
    {

        vec4 new_vertices[8] = {
            vec4(leftWall, bottomWall, 1.0, 1.0),
            vec4(leftWall, topWall, 1.0, 1.0),
            vec4(rightWall, topWall, 1.0, 1.0),
            vec4(rightWall, bottomWall, 1.0, 1.0),
            vec4(leftWall, bottomWall, -1.0, 1.0),
            vec4(leftWall, topWall, -1.0, 1.0),
            vec4(rightWall, topWall, -1.0, 1.0),
            vec4(rightWall, bottomWall, -1.0, 1.0),
        };

        for (int i = 0; i < 8; i++)
        {
            vertices[i] = new_vertices[i];
        }
    }

    // Assign colors to the walls
    void color()
    {
        update();
        // 1 and 4 for colors
        quad(2, 6, 7, 3, 7);
        quad(3, 7, 4, 0, 7);
        quad(6, 2, 1, 5, 7);
        quad(4, 7, 6, 5, 7);
        quad(5, 1, 0, 4, 1);


        Index = 0;
    }

} // namespace Walls

// Assign colors to the walls
void readppm() {


    char first_line[100];

    int a1, a2, a3 = 0;   //row, column and max values in ppm
    int c1, c2, c3; //color values in ppm

    const char* filenames[] = {
        "2k_sun.ppm",
        "2k_mercury.ppm",
        "2k_venus_surface.ppm",
        "2k_earth_daymap.ppm",
        "2k_mars.ppm",
        "2k_jupiter.ppm",
        "2k_saturn.ppm",
        "2k_uranus.ppm",
        "2k_neptune.ppm",
        "2k_moon.ppm",
        "2k_stars_milky_way2.ppm"
    };


    //reading basketball.ppm

    for (int k = 0; k < 10; k++) {
        FILE* fp = fopen(filenames[k], "r");
        if (fp == NULL) {
            exit(EXIT_FAILURE);
        }

        fscanf(fp, "%s", first_line);
        fscanf(fp, "%d %d %d", &a1, &a2, &a3);

        for (int i = 0; i < a1; i++) {
            for (int j = 0; j < a2; j++) {
                fscanf(fp, "%d %d %d", &c1, &c2, &c3);
                (*images[k])[i][j][0] = c1;
                (*images[k])[i][j][1] = c2;
                (*images[k])[i][j][2] = c3;
            }
        }

        fclose(fp);
    }

    // Special case for the 10th image
    FILE* fp = fopen(filenames[10], "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%s", first_line);
    fscanf(fp, "%d %d %d", &a1, &a2, &a3);

    for (int i = 0; i < a1; i++) {
        for (int j = 0; j < a2; j++) {
            fscanf(fp, "%d %d %d", &c1, &c2, &c3);
            (*image10_ptr)[i][j][0] = c1;
            (*image10_ptr)[i][j][1] = c2;
            (*image10_ptr)[i][j][2] = c3;
        }
    }

    fclose(fp);


}
void init()
{

    // Initializing the objects
    glEnable(GL_COLOR_MATERIAL);
    wall::color();

    sphere::tetrahedron(4);


    GLuint program;

    //switching between texture and shade mode
    if (displayMode == 0) {
        program = InitShader("vshader.glsl", "fshader.glsl");
    }
    else {
        //switching between shade modes
        if (shadeMode == 0) {
            program = InitShader("vshader_gouraud.glsl", "fshader_gouraud.glsl");
        }

        else if (shadeMode == 1) {
            program = InitShader("vshader-phong.glsl", "fshader-phong.glsl");
        }
    }


    glUseProgram(program);



    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");
    TextureFlagLoc = glGetUniformLocation(program, "TextureFlag");

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint vColor = glGetAttribLocation(program, "vColor");

    //normal and texture coordinates added
    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");

    mat4 projection;

    //Perspective projection
    projection = Perspective(FOV, 1.0, z_near, z_far);

    // Create a vertex array object
    glGenVertexArrays(NUM_OBJECTS, vao);


    // walls attributes added to buffer
    glBindVertexArray(vao[0]);

    glGenBuffers(1, &wall::buffer);
    glBindBuffer(GL_ARRAY_BUFFER, wall::buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall::points) + sizeof(wall::colors) + sizeof(wall::normals) + sizeof(wall::tex_coords),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(wall::points), wall::points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(wall::points), sizeof(wall::colors),
        wall::colors);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(wall::points) + sizeof(wall::colors), sizeof(wall::normals),
        wall::normals);

    glBufferSubData(GL_ARRAY_BUFFER, sizeof(wall::points) + sizeof(wall::colors)
        + sizeof(wall::normals), sizeof(wall::tex_coords), wall::tex_coords);

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTexCoord);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(wall::points)));

    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(wall::colors) + sizeof(wall::points)));

    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(wall::colors) + sizeof(wall::points) + sizeof(wall::normals)));

    // sphere attributes added to buffer

    for (int i = 1; i < 11; i++) {
        glBindVertexArray(vao[i]);
        glGenBuffers(1, &sphere::buffer);
        glBindBuffer(GL_ARRAY_BUFFER, sphere::buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere::points) + sizeof(sphere::colors) +
            sizeof(sphere::normals) + sizeof(sphere::tex_coords), NULL, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sphere::points), sphere::points);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere::points), sizeof(sphere::colors),
            sphere::colors);

        glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere::points) + sizeof(sphere::colors),
            sizeof(sphere::normals), sphere::normals);  //normals added

        glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere::points) + sizeof(sphere::colors)
            + sizeof(sphere::normals), sizeof(sphere::tex_coords), sphere::tex_coords);  //texcoords added


        glEnableVertexAttribArray(vPosition);
        glEnableVertexAttribArray(vColor);
        glEnableVertexAttribArray(vNormal);
        glEnableVertexAttribArray(vTexCoord);

        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(sizeof(sphere::colors)));

        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(sizeof(sphere::colors) + sizeof(sphere::points)));

        glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(sizeof(sphere::colors) + sizeof(sphere::points) + sizeof(sphere::normals)));
    }




    //Standing sphere:




    glEnable(GL_COLOR_MATERIAL);
    // Light parametes
    glGenTextures(11, textures);

    //Basketball texture 
    for (int i = 0; i < 10; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 1024, 0,
            GL_RGB, GL_UNSIGNED_BYTE, images[i]);
        glGenerateMipmap(GL_TEXTURE_2D);

    }


    glBindTexture(GL_TEXTURE_2D, textures[10]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image10);

    //minmapping
    glGenerateMipmap(GL_TEXTURE_2D);



    //switching between texture objects



    //light replacement, doesn't follows object

    /*if (lightMode == 0) {
        light_position = point4(0.0, 0.0, -2.0, 1.0);
    }
    else {
        light_position = point4(0.5, 0.0, -2.0, 1.0);
    }*/


    color4 light_ambient(0.2, 0.2, 0.2, 1.0); // L_a
    color4 light_diffuse(1, 1, 1, 1.0); // L_d
    color4 light_specular(0.2, 0.2, 0.2, 1.0); // L_s

    color4 material_ambient(1.0, 1.0, 1.0, 1.0); // k_a
    color4 material_diffuse(1, 1, 1, 1.0); // k_d
    color4 material_specular(1.0, 1.0, 1.0, 1.0); // k_s

    float  material_shininess;

    //material switch
    if (materialMode == 0) {
        material_shininess = 10;    //metal material
    }
    else {
        material_shininess = 200;   //plastic material
    }


    //Component switch
    if (componentMode == 1) {
        light_specular = 0;
    }
    else if (componentMode == 2) {
        light_diffuse = 0;
    }
    else if (componentMode == 3) {
        light_ambient = 0;
    }


    color4 ambient_product = light_ambient * material_ambient; // k_a * L_a
    color4 diffuse_product = light_diffuse * material_diffuse; // k_d * L_d
    color4 specular_product = light_specular * material_specular; // k_s * L_s



    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);

    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);

    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);




    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
    glUniform1i(TextureFlagLoc, textureFlag);

    //depth test and culling

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}

//---------------------------------------------------------------------
//
// display

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rendering walls first
    mat4 model_view = Translate(vec3(0.0, 0.0, -2.0)) * Scale(1.0, 1.0, 1.0);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, wall::buffer);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    glBindTexture(GL_TEXTURE_2D, textures[10]);
    glDrawArrays(GL_TRIANGLES, 0, wall::numVertices);

    displacement += velo;

    // increase the displacement vector in every frame

    //light replacement
    if (lightMode == 0) {
        model_view = Translate(vec3(0.5, 0.0, 0.0));
        glUniformMatrix4fv(denemee, 1, GL_TRUE, model_view);
    }
    else {
        model_view = Translate(vec3(displacement.x, displacement.y, displacement.z));
        glUniformMatrix4fv(denemee, 1, GL_TRUE, model_view);

    }


    // rotate cube and sphere


    for (int i = 0; i < 10; i++) {
        /*model_view = (Translate(TOP_LEFT_FRONT_CORNER + (i-1) * vec3(0.2, 0, 0)) *
            Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR) * RotateX(Theta[Xaxis]) *
            RotateY(Theta[Yaxis]) *
            RotateZ(Theta[Zaxis]) *
            RotateZ(180)* Translate((i - 1) * vec3(0.2, 0, 0)));*/

        model_view = (Translate(TOP_LEFT_FRONT_CORNER)* RotateX(ThetaR[0]) * RotateZ(ThetaR[2]) *  RotateX(Theta[i][Xaxis]) *
            RotateY(Theta[i][Yaxis]) *
            RotateZ(Theta[i][Zaxis]) *
            RotateZ(180) * Translate(distance[i]) *
            Scale(scales[i] / 2, scales[i] / 2, scales[i] / 2));
        if (i == 9) {
            model_view = ( Translate(TOP_LEFT_FRONT_CORNER) * RotateX(ThetaR[0]) * RotateZ(ThetaR[2]) * RotateX(Theta[3][Xaxis]) *
                RotateY(Theta[3][Yaxis]) *
                RotateZ(Theta[3][Zaxis]) *
                RotateZ(180) * Translate(distance[3]) * RotateY(Theta[10][Yaxis]) *
                Scale(scales[i] / 2, scales[i] / 2, scales[i] / 2))  * Translate(vec3(4, 0, 0));

            glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
            glBindVertexArray(vao[i + 1]);
            glBindBuffer(GL_ARRAY_BUFFER, sphere::buffer);

            glBindTexture(GL_TEXTURE_2D, textures[i]); //set current texture
            glDrawArrays(GL_TRIANGLES, 0, sphere::numVertices);

            for (int a = 0; a < 21; a++) {
                model_view = (Translate(TOP_LEFT_FRONT_CORNER) * RotateX(ThetaR[0]) * RotateZ(ThetaR[2]) * RotateY(Theta[6][Yaxis]) *
                    RotateZ(180) * Translate(distance[6]) *
                    RotateY(a*(360/20)) * Translate(vec3(0.15, 0, 0)) * Scale(0.04, 0.01, 0.04));

                glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

                glDrawArrays(GL_TRIANGLES, 0, sphere::numVertices);
            }

        }

        else{
            glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
            glBindVertexArray(vao[i + 1]);
            glBindBuffer(GL_ARRAY_BUFFER, sphere::buffer);

            glBindTexture(GL_TEXTURE_2D, textures[i]); //set current texture
            glDrawArrays(GL_TRIANGLES, 0, sphere::numVertices);

            //Switching to wireframe
            if (dmode == 0)
            {
                glDrawArrays(GL_LINE_LOOP, 0, sphere::numVertices);
            }
            else
            {
                glDrawArrays(GL_TRIANGLES, 0, sphere::numVertices);
            }

        }
        
    }





    glFlush();

}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
    int mods)
{
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        exit(EXIT_SUCCESS);
        break;
    }

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        displacement = TOP_LEFT_FRONT_CORNER; // set object to top left
    }


    if ((key == GLFW_KEY_S || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) && action == GLFW_PRESS)
    {
        shadeMode = (shadeMode + 1) % 2; // change shading mode
        init();
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        displayMode = (displayMode + 1) % 2; // change display mode
        init();
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        materialMode = (materialMode + 1) % 2; // change material mode
        init();
    }
    if ((key == GLFW_KEY_T || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
    {
        objectMode = (objectMode + 1) % 2; // change object
        init();
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        componentMode = (componentMode + 1) % 4; // change light component
        init();
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        lightMode = (lightMode + 1) % 2; // set object to top left
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        sphere::colorInt = (sphere::colorInt + 1) % 7;
        printf("%d\n", sphere::colorInt);
        init();

    }
    if (key == GLFW_KEY_0)
    {
        ThetaR[0] += 2;

    }
    if (key == GLFW_KEY_9)
    {
        ThetaR[0] -= 2;

    }
    if (key == GLFW_KEY_8)
    {
        ThetaR[2] += 2;

    }
    if (key == GLFW_KEY_7)
    {
        ThetaR[2] -= 2;

    }

    if (key == GLFW_KEY_6)
    {
       zoom[2] += 0.1;

    }
    if (key == GLFW_KEY_5)
    {
        zoom[2] -= 0.1;

    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        dmode++; // change drawing mode
        if (dmode == 2)
        {
            dmode = 0;
        }
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        std::cout << "_________________________________________________" << std::endl;
        std::cout << "Press H => Print the commands" << std::endl;
        std::cout << "Press I => Reset the ball to initial position" << std::endl;
        std::cout << "Press V => Change the drawing mode" << std::endl;
        std::cout << "Press M => Change the material mode" << std::endl;
        std::cout << "Press S or Arrow Up/Down => Change the shading mode" << std::endl;
        std::cout << "Press R => Change the display mode" << std::endl;
        std::cout << "Press T or Arrow Left/Right => Change the object" << std::endl;
        std::cout << "Press C => Change the component mode" << std::endl;
        std::cout << "Press L => Change the light mode" << std::endl;
        std::cout << "Press Q => Quit the program" << std::endl;
        std::cout << "_________________________________________________" << std::endl;
    }
}



void resize_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);

    int old_w = w;
    int old_h = h;

    GLfloat aspect_ratio = (GLfloat)old_w / (GLfloat)old_h;

    if (old_w <= old_h)
    {
        leftWall = -1.0;
        rightWall = 1.0;

        bottomWall = -1.0 / aspect_ratio;
        topWall = 1.0 / aspect_ratio;
    }
    else
    {
        bottomWall = -1.0;
        topWall = 1.0;

        leftWall = -1.0 * aspect_ratio;
        rightWall = 1.0 * aspect_ratio;
    }

    wall::color();
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, wall::buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(wall::points), wall::points);
}

// Update function. Check for wall boundarie's
// and change the direction of the object if it hits the wall
void update(void)
{
    velo[0] = 0.009;
    velo[1] = 0.006;
    velo[2] = 0.003;

    if (displacement[0] > 0.75)
    {
        direction[0] = 0;
    }

    if (displacement[1] > 0.750)
    {
        direction[1] = 0;
    }

    if (displacement[2] > -0.75)
    {
        direction[2] = 0;
    }

    if (displacement[2] < -2.75)
    {
        direction[2] = 1;
    }

    if (displacement[0] < -0.75)
    {
        direction[0] = 1;
    }

    if (displacement[1] < -0.75)
    {
        direction[1] = 1;
    }

    if (direction[0] == 0)
    {
        velo[0] = -0.009;
    }
    if (direction[1] == 0)
    {
        velo[1] = -0.006;
    }
    if (direction[2] == 0)
    {
        velo[2] = -0.003;
    }

    for (int i = 0; i < 11; i++) {
        Theta[i][1] += thetas[i];
    }

}

// main
int main()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Project", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    readppm();      //reading ppm files here

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, resize_callback);

    init();
    double frameRate = 120, currentTime, previousTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1 / frameRate)
        {
            previousTime = currentTime;
            update();
        }

        glfwPollEvents();
        display();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}