#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>

typedef struct VertexObject {
    unsigned int VBO, VAO, EBO; // Vertex Buffer, Vertex Array, Element Buffer
    unsigned int texture;
    unsigned int vert_count;
} VertexObject;

void initVertArray(VertexObject* vertobj, float vertices[], unsigned int indices[], unsigned long vertices_size, unsigned long indices_size) {
    glGenVertexArrays(1, &vertobj->VAO);
    glGenBuffers(1, &vertobj->VBO);
    glGenBuffers(1, &vertobj->EBO);

    glBindVertexArray(vertobj->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertobj->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertobj->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

VertexObject* colorRect(float width, float height, vec3 color) {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions             // colors         
         width,  height, 0.0f,   color[0], color[1], color[2],  // top right
         width, -height, 0.0f,   color[0], color[1], color[2],  // bottom right
        -width, -height, 0.0f,   color[0], color[1], color[2],  // bottom left
        -width,  height, 0.0f,   color[0], color[1], color[2],  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    VertexObject* vertobj = malloc(sizeof(VertexObject));
    vertobj->vert_count = sizeof(indices)/sizeof(unsigned int);
    initVertArray(vertobj, vertices, indices, sizeof(vertices), sizeof(indices));
    
    return vertobj;
}

VertexObject* colorRectOutline(float width, float height, float border, vec3 color) {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // TOP
        // positions                        // colors         
         width, height,               0.0f, color[0], color[1], color[2],  // 0 top right
         width-border, height-border, 0.0f, color[0], color[1], color[2],  // 1 bottom right
        -width+border, height-border, 0.0f, color[0], color[1], color[2],  // 2 bottom left
        -width, height,               0.0f, color[0], color[1], color[2],  // 3 top left

        // BOTTOM
        // positions                         // colors         
         width-border, -height+border, 0.0f, color[0], color[1], color[2],  // 4 top right
         width, -height,               0.0f, color[0], color[1], color[2],  // 5 bottom right
        -width, -height,               0.0f, color[0], color[1], color[2],  // 6 bottom left
        -width+border, -height+border, 0.0f, color[0], color[1], color[2],  // 7 top left
    };
    unsigned int indices[] = {
        // TOP
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle

        // RIGHT
        0, 5, 1, // first triangle
        5, 4, 1, // second triangle

        // BOTTOM
        4, 5, 7, // first triangle
        5, 6, 7, // second triangle

        // LEFT
        7, 6, 3, // first triangle
        3, 2, 7, // second triangle

    };
    
    VertexObject* vertobj = malloc(sizeof(VertexObject));
    vertobj->vert_count = sizeof(indices)/sizeof(unsigned int);
    initVertArray(vertobj, vertices, indices, sizeof(vertices), sizeof(indices));
    
    return vertobj;
}

VertexObject* colorDashedLine(float length, float width, int dashes, float spacing, vec3 color) {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[dashes*24];
    unsigned int indices[dashes*6];

    float full_dash = length/(float)dashes;
    float half_length = (length/2.0f);
    spacing /= 2.0f;

    for (int i=0; i<dashes; i++) {
        float new_vertices[] = {
            // positions                                   // colors         
             width, full_dash*(i+1)-spacing-half_length,  0.0f, color[0], color[1], color[2],  // 0 top right
             width, full_dash*i+spacing-half_length,      0.0f, color[0], color[1], color[2],  // 1 bottom right
            -width, full_dash*i+spacing-half_length,      0.0f, color[0], color[1], color[2],  // 2 bottom left
            -width, full_dash*(i+1)-spacing-half_length,  0.0f, color[0], color[1], color[2],  // 3 top left
        };

        unsigned int new_indices[] = {
            (i*4),   (i*4)+1, (i*4)+3, // first triangle
            (i*4)+1, (i*4)+2, (i*4)+3, // second triangle
        };

        memcpy(vertices+(i*24), new_vertices, sizeof(new_vertices));
        memcpy(indices+(i*6),   new_indices,  sizeof(new_indices));
    }
    
    VertexObject* vertobj = malloc(sizeof(VertexObject));
    vertobj->vert_count = sizeof(indices)/sizeof(unsigned int);
    initVertArray(vertobj, vertices, indices, sizeof(vertices), sizeof(indices));
    
    return vertobj;
}


VertexObject* textureRect() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    struct VertexObject* vertobj = malloc(sizeof(VertexObject));

    glGenVertexArrays(1, &vertobj->VAO);
    glGenBuffers(1, &vertobj->VBO);
    glGenBuffers(1, &vertobj->EBO);

    glBindVertexArray(vertobj->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertobj->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertobj->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    glGenTextures(1, &vertobj->texture);
    glBindTexture(GL_TEXTURE_2D, vertobj->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);
    return vertobj;
}


#endif