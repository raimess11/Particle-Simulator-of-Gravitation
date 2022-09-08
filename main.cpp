#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>

#include "C:\Development\SDL2-2.24.0\i686-w64-mingw32\include\SDL2\SDL.h"
// math
const double PI = 3.14159265;

//window setting
const int WIDTH = 640*1.5, LENGTH = 480*1.5;

//render scale setting
int scale = 4;
int x_scale= 1 * scale;
int y_scale= 1 * scale;


//refresh rate things
double CPU = 0;
double vdeltaTime = 0;
float fps = 0;
float fps_limit = 120;
float delay =0 ;
clock_t deltaTime= 0;

//process delta
clock_t process_time;
double delta_process;
clock_t begin_process_time;

//input
bool holdLMB;

//function
float phytagoras(float a, float b)
{
    return sqrt(a*a + b*b);
}
//classes
class Vector2D{
public:
    double x = 0;
    double y = 0;
};

class Particle{
public:
    Vector2D position;

    Vector2D velocity;
    Vector2D direction;

    float mass = 10;
    bool statis;

    SDL_Color color;

    float disctance_to(float x1, float y1)
    {
        return phytagoras(x1-position.x,y1-position.y);
    }

    void translation(Vector2D* F, Vector2D dir, float a){
            F->x += a * dir.x;
            F->y += a * dir.y;
    }

    void move_and_slide(){
        position.x += velocity.x * delta_process;
        position.y += velocity.y * delta_process;
    }
};

class Tab_Object{
public:
    Particle id[1000];
    int len;

    void append(Particle p) {
        id[len] = p;
        std::cout<<p.position.x<<"," <<p.position.y<<std::endl;
        len++;
    }
};

class Tab_Vector2D{
public:
    Vector2D id[1000];
    int len;
    void append(Vector2D p) {
        id[len] = p;
        len++;
    }
};

void Gforce(Tab_Object tab, Particle* POV,int current_index){
        Vector2D vel = POV->velocity;
        for(int i=0; i<tab.len; i++){
            Vector2D dir;
            if(i != current_index ){
                if (POV->disctance_to(tab.id[i].position.x,tab.id[i].position.y) >1 || true ){
                    dir.x = (tab.id[i].position.x - POV->position.x) / POV->disctance_to(tab.id[i].position.x,tab.id[i].position.y);
                    dir.y = (tab.id[i].position.y - POV->position.y) / POV->disctance_to(tab.id[i].position.x,tab.id[i].position.y);
                    if(abs(tab.id[i].disctance_to(POV->position.x,POV->position.y))>1){
                        POV->translation(&vel,dir,(10 * tab.id[i].mass) / (pow(tab.id[i].disctance_to(POV->position.x,POV->position.y),2)));
                    }
                }
                        //std::cout<<"pulled by gravity"<<std::endl;
            }
        }
        POV->velocity = vel;
        if( deltaTime%200 > 136 ){
        }

}
//declare the particle
Tab_Object object;

Tab_Vector2D coordinate;

int main( int argc, char *argv[] )
{


    if(SDL_Init(SDL_INIT_EVERYTHING)<0){
        std::cout<<"SDL could not initialize! Error: "<<SDL_GetError()<<std::endl;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_CreateWindowAndRenderer(WIDTH,LENGTH,0,&window,&renderer);
    SDL_RenderSetScale(renderer,x_scale,y_scale);

    if(window == NULL){
        std::cout<<"could not create window! Error: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }
    SDL_Event event;

    std::cout<<object.len;
    //main loop
    while(true){
        //initialize begin time for FPS reason
        clock_t begintime = clock();

        //do process here
        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                break;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT){

                    holdLMB=true;

                    Particle new_particle;
                    int x;
                    int y;
                    SDL_GetMouseState(&x,&y);
                    new_particle.position.x = x/x_scale;
                    new_particle.position.y = y/y_scale;
                    new_particle.color.b = 200;
                    new_particle.color.a = 255;
                    std::cout<<"summon proton at: "<<x/x_scale<<std::endl;
                    object.append(new_particle);
                }
                if(event.button.button == SDL_BUTTON_MIDDLE){

                    holdLMB=true;

                    Particle new_particle;
                    int x;
                    int y;
                    SDL_GetMouseState(&x,&y);
                    new_particle.position.x = x/x_scale;
                    new_particle.position.y = y/y_scale;
                    new_particle.color.r = 255;
                    new_particle.color.g = 255;
                    new_particle.color.b = 255;
                    new_particle.color.a = 255;
                    new_particle.statis = true;
                    std::cout<<"summon proton at: "<<x/x_scale<<std::endl;
                    object.append(new_particle);
                }
                if(event.button.button == SDL_BUTTON_RIGHT){
                    holdLMB=true;

                    Particle new_particle;
                    int x;
                    int y;
                    SDL_GetMouseState(&x,&y);
                    new_particle.position.x = x/x_scale;
                    new_particle.position.y = y/y_scale;
                    new_particle.mass = 0;
                    new_particle.color.r = 150;
                    new_particle.color.a = 255;
                    new_particle.velocity.x = 30;
                    std::cout<<"summon electron at: "<<x/x_scale<<std::endl;
                    object.append(new_particle);
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    holdLMB=false;
                }
            }
        }



        //render process
        if(vdeltaTime > 1000/fps_limit){
            process_time = clock();
            delta_process = process_time - begin_process_time;
            delta_process /= 1000;
            begin_process_time = process_time;
            //input handle
            if(holdLMB){

            }
            //processing object
            for (int i=0; i<object.len; i++){
                    if(true){
                        //object.id[i].reaction();
                        Gforce(object,&object.id[i],i);
                        //std::cout<<object.id[i].position.x<<"'"<<object.id[i].position.y<<std::endl;
             //::cout<<"delta of object: "<<delta_process<<std::endl;
                    }
                }
            for (int i=0; i<object.len; i++){
                //bool free = true;
                    /*for (int j=0; j<coordinate.len; j++){
                        if(j==i){continue;}
                        if(phytagoras(object.id[i].velocity.x - coordinate.id[j].x, object.id[i].velocity.y - coordinate.id[j].y) < 1){
                            object.id[i].velocity.x -= object.id[i].velocity.x * (1/phytagoras(object.id[i].velocity.x,object.id[i].velocity.y));
                            object.id[i].velocity.y -= object.id[i].velocity.y * (1/phytagoras(object.id[i].velocity.x,object.id[i].velocity.y));
                            break;
                        }
                    }*/
                if(!object.id[i].statis){
                    object.id[i].move_and_slide();
                }
                //coordinate.append(object.id[i].position);
            }

               // std::cout<<"delta of object: "<<delta_process<<std::endl;

            //render
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);

            //render every particle
            for (int i=0; i<object.len; i++){
                SDL_SetRenderDrawColor(renderer,object.id[i].color.r,object.id[i].color.g,object.id[i].color.b,object.id[i].color.a);
                //std::cout<<"position of object: "<<object.id[i].position.x<< " , "<< object.id[i].position.y <<std::endl;
               // std::cout<<"velocity of object: "<<object.id[i].velocity.x<< " , "<< object.id[i].velocity.y <<std::endl;
                //std::cout<<"angle of object: "<<object.id[i].angle<<std::endl;
                SDL_RenderDrawPoint(renderer,object.id[i].position.x,object.id[i].position.y);
            }
            SDL_RenderPresent(renderer);
            fps++;
            vdeltaTime=0;
            delta_process=begintime - process_time;
        }




        //initialize end time for FPS reason
        clock_t endtime = clock();


        //count delta time
        deltaTime += endtime - begintime;
        vdeltaTime += endtime - begintime;
        //count frame
        CPU++;
        //get fps every 1 sec
        if( deltaTime >= 1000 ){
            std::cout<<"CPU: "<<CPU<<std::endl;
            std::cout<<"FPS: "<<fps<<std::endl;
            std::cout<<"object count: "<<object.len<<std::endl;
            deltaTime=0;
            CPU=0;
            fps=0;
        }
        //SDL_Delay((1000/120)-deltaTime);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
