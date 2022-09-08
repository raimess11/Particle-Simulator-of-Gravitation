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
int scale = 8;
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
    float torsi;
    float rotation_momentum;
    float angle=0;
    float speed;
    float max_speed=100;
    float acceleration=0.5;
    bool charge;

    float disctance_to(float x1, float y1)
    {
        return phytagoras(x1-position.x,y1-position.y);
    }

    void move_and_slide(Vector2D vel){

        position.x += vel.x * delta_process;
        position.y += vel.y * delta_process;
    }

    void translation(Vector2D* vel, Vector2D dir, float accel){
        if (phytagoras(vel -> x, vel-> y) < max_speed){
            vel->x += accel * dir.x;
            vel->y += accel * dir.y;
        }

    }

    void get_direction(double *x, double *y, float teta){
    *x = sin(teta*(PI/180));
    *y = cos(teta*(PI/180));
    }

    void left_turn(){
        torsi = 0.07;
    }
    void right_turn(){
        torsi = -0.07;
    }

    void rotation(){

        if (abs(rotation_momentum) < 0.7){
            rotation_momentum += torsi;
        }

        angle += rotation_momentum;
        angle = int(angle*1000)%(360*1000);
        angle = angle/1000;

    if(position.x>(WIDTH/x_scale)-20/x_scale){
       // if (angle >= 0 && angle < 180){left_turn();}
       // else {right_turn();}
    }
    if(position.y<20/y_scale){
        //if (angle >= 90 && angle < 270){left_turn();}
        //else {right_turn();}
    }
    if(position.x<20/x_scale){
        //if (angle >= 180 && angle < 360){left_turn();}
        //else {right_turn();}
    }
    if(position.y>(LENGTH/y_scale)-20/y_scale){
        //if (angle >= 270 && angle < 90){left_turn();}
        //else {right_turn();}
    }
    }



    void reaction(){
        get_direction(&direction.x,&direction.y,angle);
        translation(&velocity,direction,acceleration);
        move_and_slide(velocity);

    }

};

class Tab_Object{
public:
    Particle id[10000];
    int len;

    void append(Particle p) {
        id[len] = p;
        std::cout<<p.position.x<<"," <<p.position.y<<std::endl;
        len++;
    }
};

void Gforce(Tab_Object tab, Particle* POV,int current_index){
        Vector2D vel;
        Vector2D dir;
        for(int i=0; i<tab.len; i++){
            if(i != current_index ){
                if (tab.id[i].disctance_to(POV->position.x,POV->position.y) < 200/scale){
                    dir.x = (tab.id[i].position.x - POV->position.x) / POV->disctance_to(tab.id[i].position.x,tab.id[i].position.y);
                    dir.y = (tab.id[i].position.y - POV->position.y) / POV->disctance_to(tab.id[i].position.x,tab.id[i].position.y);
                    if(tab.id[i].disctance_to(POV->position.x,POV->position.y)!=0){
                        POV->translation(&vel,dir,10 / tab.id[i].disctance_to(POV->position.x,POV->position.y));
                    }
                }
                        //std::cout<<"pulled by gravity"<<std::endl;
            }
        }
        POV->move_and_slide(vel);

}
//declare the particle
Tab_Object object;


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
                    std::cout<<"button pressed on: "<<x/x_scale<<std::endl;
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
                    if(object.id[i].charge){
                        //object.id[i].reaction();
                        Gforce(object,&object.id[i],i);
             //::cout<<"delta of object: "<<delta_process<<std::endl;
                    }
                }
               // std::cout<<"delta of object: "<<delta_process<<std::endl;

            //render
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);

            //render every particle
            for (int i=0; i<object.len; i++){
                SDL_SetRenderDrawColor(renderer,0,100,0,255);
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
