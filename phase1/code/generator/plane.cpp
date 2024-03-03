#include "plane.hpp"
                                                                          //o length é o tamanho do lado do plano
                                                                          //o divisions é o número de divisões do plano
FIGURE generate_plane_XZ(int length, int divisions, float h, int baixo) { //o h é a altura do plano em relação ao eixo y
                                                                          //o baixo é para inverter o plano
    FIGURE f = create_figure(PLANE, length, divisions); 

    float dimension = (float)length / 2; //isto serve para centrar o plano no centro do eixo (0,0,0) 
    float division = (float)length / divisions; //isto serve para dividir o plano nas várias partes que queremos gerar

    //agr vamos criar os pontos do plano, depois de sabermos quantas divisões queremos e termos a referença do centro do plano

    float x1 = -dimension, z1 = -dimension;                         //isto é o ponto inicial do plano, ou seja, está no canto inferior esquerdo
    float x2 = -dimension, z2 = -dimension + division;              //isto é o ponto que está à direita do ponto inicial, ou seja, está no canto superior esquerdo
    float x3 = -dimension + division, z3 = -dimension;              //isto é o ponto que está em baixo do ponto inicial, ou seja, está no canto inferior direito
    float x4 = -dimension + division, z4 = -dimension + division;   //isto é o ponto que está à direita do ponto inicial, ou seja, está no canto superior direito

    float Xs[4] = {x1, x2, x3, x4}; //array dos X's
    float Zs[4] = {z1, z2, z3, z4}; //arrays dos Z's

    //isto serve para inverter, pois vamos usar isto para criar a box (verificar mesmo se queremos usar isto, caso não seja necessário, apagar)
    if (baixo == 1){
        Xs[1] = x3;
        Xs[2] = x2;
        Zs[1] = z3;
        Zs[2] = z2;
    }

    //ao usarmos estes dois ciclos for, vamos criar os vários pontos do plano de maneira a que fiquem divididos nas várias partes que queremos

    for (int linha = 0; linha < divisions; linha++){ //itera sobre o eixo do Z (neste caso)
        for (int coluna = 0; coluna < divisions; coluna++){ //itera sobre o eixo do X
            // Primeiro triângulo do quadrado
            POINT p1 = new_point(Xs[0] + coluna * division, h, Zs[0]); 
            POINT p2 = new_point(Xs[1] + coluna * division, h, Zs[1]);
            POINT p3 = new_point(Xs[2] + coluna * division, h, Zs[2]);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(Xs[1] + coluna * division, h, Zs[1]);
            POINT p5 = new_point(Xs[3] + coluna * division, h, Zs[3]);
            POINT p6 = new_point(Xs[2] + coluna * division, h, Zs[2]);


            // Inicialize vetores vazios
            std::vector<POINT> vertices;
            std::vector<int> indices;

            // Adicione os elementos um a um
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p4);
            vertices.push_back(p5);
            vertices.push_back(p6);

            // Adicione os índices um a um
            indices.push_back(get_size_vertices(f) - 6);
            indices.push_back(get_size_vertices(f) - 5);
            indices.push_back(get_size_vertices(f) - 4);
            indices.push_back(get_size_vertices(f) - 3);
            indices.push_back(get_size_vertices(f) - 2);
            indices.push_back(get_size_vertices(f) - 1);

            add_vertexs(f, vertices);
            add_indexs(f, indices);

        }
        Zs[0] += division;
        Zs[1] += division;
        Zs[2] += division;
        Zs[3] += division;
    }

    return f;
}

FIGURE generate_plane_XY(int length, int divisions, float h, int baixo) {
    FIGURE f = create_figure(PLANE, length, divisions);

    float dimension = (float)length / 2;
    float division = (float)length / divisions;
    float x1 = -dimension, y1 = -dimension,
          x2 = -dimension, y2 = -dimension + division,
          x3 = -dimension + division, y3 = -dimension,
          x4 = -dimension + division, y4 = -dimension + division;

    float arrx[4] = {x1, x2, x3, x4};
    float arry[4] = {y1, y2, y3, y4};

    if (baixo == 1){
        arrx[1] = x3;
        arrx[2] = x2;
        arry[1] = y3;
        arry[2] = y2;
    }

    for (int linha = 0; linha < divisions; linha++){
        for (int coluna = 0; coluna < divisions; coluna++){
            // Primeiro triângulo do quadrado
            POINT p1 = new_point(arrx[0] + coluna * division, arry[0], h);
            POINT p2 = new_point(arrx[1] + coluna * division, arry[1], h);
            POINT p3 = new_point(arrx[2] + coluna * division, arry[2], h);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(arrx[1] + coluna * division, arry[1], h);
            POINT p5 = new_point(arrx[3] + coluna * division, arry[3], h);
            POINT p6 = new_point(arrx[2] + coluna * division, arry[2], h);

            // Inicialize vetores vazios
            std::vector<POINT> vertices;
            std::vector<int> indices;

            // Adicione os elementos um a um
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p4);
            vertices.push_back(p5);
            vertices.push_back(p6);

            // Adicione os índices um a um
            indices.push_back(get_size_vertices(f) - 6);
            indices.push_back(get_size_vertices(f) - 5);
            indices.push_back(get_size_vertices(f) - 4);
            indices.push_back(get_size_vertices(f) - 3);
            indices.push_back(get_size_vertices(f) - 2);
            indices.push_back(get_size_vertices(f) - 1);

            add_vertexs(f, vertices);
            add_indexs(f, indices);

        }
        arry[0] += division;
        arry[1] += division;
        arry[2] += division;
        arry[3] += division;
    }

    return f;
}

FIGURE generate_plane_YZ(int length, int divisions, float h, int baixo) {
    FIGURE f = create_figure(PLANE, length, divisions);

    float dimension = (float)length / 2, div_side = (float)length / divisions;
    float y1 = -dimension, z1 = -dimension,
          y2 = -dimension, z2 = -dimension + div_side,
          y3 = -dimension + div_side, z3 = -dimension,
          y4 = -dimension + div_side, z4 = -dimension + div_side;

    float arry[4] = {y1, y2, y3, y4};
    float arrz[4] = {z1, z2, z3, z4};

    if (baixo == 1){
        arry[1] = y3;
        arry[2] = y2;
        arrz[1] = z3;
        arrz[2] = z2;
    }

    for (int linha = 0; linha < divisions; linha++){
        for (int coluna = 0; coluna < divisions; coluna++){
            // Primeiro triângulo do quadrado
            POINT p1 = new_point(h, arry[0] + coluna * div_side, arrz[0]);
            POINT p2 = new_point(h, arry[1] + coluna * div_side, arrz[1]);
            POINT p3 = new_point(h, arry[2] + coluna * div_side, arrz[2]);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(h, arry[1] + coluna * div_side, arrz[1]);
            POINT p5 = new_point(h, arry[3] + coluna * div_side, arrz[3]);
            POINT p6 = new_point(h, arry[2] + coluna * div_side, arrz[2]);

            // Inicialize vetores vazios
            std::vector<POINT> vertices;
            std::vector<int> indices;

            // Adicione os elementos um a um
            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p4);
            vertices.push_back(p5);
            vertices.push_back(p6);

            // Adicione os índices um a um
            indices.push_back(get_size_vertices(f) - 6);
            indices.push_back(get_size_vertices(f) - 5);
            indices.push_back(get_size_vertices(f) - 4);
            indices.push_back(get_size_vertices(f) - 3);
            indices.push_back(get_size_vertices(f) - 2);
            indices.push_back(get_size_vertices(f) - 1);

            add_vertexs(f, vertices);
            add_indexs(f, indices);

        }
        arrz[0] += div_side;
        arrz[1] += div_side;
        arrz[2] += div_side;
        arrz[3] += div_side;
    }

    return f;
}

