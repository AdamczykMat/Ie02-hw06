#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(const int fps, const std::string& path) //make an initializer list for fps_
    {

        fps_ = fps;
        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << std::endl;}
        setOrigin(25,0);
        setPosition(25,1);
        setTexture(texture_);
        setTextureRect(sf::IntRect(50, 0, 50, 37));
    }

    void setSpeed(const int& x_speed, const int& y_speed, const int & ro_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }

    void MoveInDirection(const sf::Time &elapsed, const sf::Keyboard::Key &key){
        float dt = elapsed.asSeconds();

        t_ = t_ + dt;
        std::cout << "dt: " << dt << " /// t_: " << t_ << std::endl;

        if(t_ > 1.0/fps_)
        {
            t_ = 0.0;
            fragments_index++;
            if(fragments_index == running_frames.size()) {fragments_index = 0;}

            std::cout << "frame: " << fragments_index << std::endl;
            setTextureRect(running_frames[fragments_index]);
        }

        if(key == sf::Keyboard::Left)
        {
            setScale(-1.5,1.5);
            x_speed_ = -1*abs(x_speed_);
            bouncce();
            bouncceWalls();
            move(x_speed_ * dt, 0);
        }
        if(key == sf::Keyboard::Right)
        {
            setScale(1.5,1.5);
            x_speed_ = abs(x_speed_);
            bouncce();
            bouncceWalls();
            move(x_speed_ * dt, 0);
        }

    }

    void gravitation(const sf::Time &elapsed){

        float dt = elapsed.asSeconds();
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        bouncceWalls();

        if(jumping_)
        {
            jt_ += dt;

            if(jt_ < 0.3)
                move(0,-1/pow(jt_+1.5,2));
            else
            {
                jt_ = 0.0;
                jumping_ = false;
            }
        }
        else if(!on_ground_)
        {
            gt_ += dt;
            if(rectangle_bounds.top + rectangle_bounds.height < d_bound_){
                move(0,(1/2.0)*pow(gt_,2));
            }
            else{
                gt_ = 0.0;
                on_ground_ = true;
                setPosition(rectangle_bounds.left + rectangle_bounds.width/2 ,d_bound_- rectangle_bounds.height);
            }
        }
    }

    void jump(void){
        if(on_ground_)
        {
            jumping_ = true;
            on_ground_ = false;
        }
    }

    void isOnGround(const bool &og) {on_ground_ = og;}

    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }

    void add_animation_frame(const sf::IntRect& frame){running_frames.emplace_back(frame); }

    void add_wall_bounds(const sf::FloatRect &wallB){walls_bounds.emplace_back(wallB); }

private:
    sf::Texture texture_;
    int fps_;
    int x_speed_ = 0 ;
    int y_speed_ = 0 ;
    int ro_speed_ = 0 ;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;
    float t_ = 0.0;
    float gt_ = 0.0;
    float jt_ = 0.0;

    bool on_ground_ = false;
    bool jumping_ = false;

    unsigned int fragments_index = 0;
    std::vector<sf::IntRect> running_frames;
    std::vector<sf::FloatRect> walls_bounds;


    void bouncce(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if(rectangle_bounds.top <= u_bound_){
            y_speed_ = abs(y_speed_);
        }

        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
            y_speed_ = abs(y_speed_) * -1;
        }

        if(rectangle_bounds.left <= l_bound_ ){
           x_speed_ = abs(x_speed_);
        }

        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
            x_speed_ = abs(x_speed_) * -1;
        }
    }

    void bouncceWalls(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        for(auto wall: walls_bounds)
        {
        if(rectangle_bounds.top < wall.top
          && rectangle_bounds.top  + rectangle_bounds.height > wall.top
          && rectangle_bounds.left - rectangle_bounds.width/2 < wall.left + wall.width
          //&& rectangle_bounds.left > wall.left
                )
        {
            on_ground_ = true;
             setPosition(rectangle_bounds.left + rectangle_bounds.width/2 ,wall.top - rectangle_bounds.height);}
        else on_ground_ = false;

//        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
//            y_speed_ = abs(y_speed_) * -1;
//        }

//        if(rectangle_bounds.left <= l_bound_ ){
//           x_speed_ = abs(x_speed_);
//        }

//        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
//            x_speed_ = abs(x_speed_) * -1;
//        }

        }
    }

};



int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    //window.setFramerateLimit(60);

    AnimatedSprite hero(10, "Character\\character.png");
    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);
    hero.setSpeed(50,50,10);


    //hero.add_animation_frame(sf::IntRect(0, 0, 50, 37)); // hero standing frame 1
    //hero.add_animation_frame(sf::IntRect(50, 0, 50, 37)); // hero standing frame 2
    //hero.add_animation_frame(sf::IntRect(100, 0, 50, 37)); // hero standing frame 3
    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 2
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 3
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 4
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 5
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 6

    sf::Clock clock;


    //background
    sf::Texture texture_sky;
    sf::Texture texture_sea;
    sf::Texture texture_clouds;

    if (!texture_sky.loadFromFile("Map\\sky.png")  || !texture_sea.loadFromFile("Map\\sea.png") || !texture_clouds.loadFromFile("Map\\clouds.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    texture_sky.setRepeated(true);
    sf::Sprite sky;
    sky.setTexture(texture_sky);
    sky.setTextureRect(sf::IntRect(0, 0, 800, 600));

    texture_sea.setRepeated(true);
    sf::Sprite sea;
    sea.setTexture(texture_sea);
    sea.setTextureRect(sf::IntRect(0, 0, 800, 250));
    sea.setPosition(0,350);

    texture_clouds.setRepeated(true);
    sf::Sprite clouds;
    clouds.setTexture(texture_clouds);
    clouds.setTextureRect(sf::IntRect(0, 0, 800, 200));
    clouds.setPosition(0,150);

    //walls

    sf::Texture texture_wall;
    if(!texture_wall.loadFromFile("wall.png")) {
        return 1; }

    std::vector<sf::Sprite> walls;

    sf::Sprite wall;

    //wall_1
    wall.setTexture(texture_wall);
    wall.setScale(1, 1);
    wall.setPosition(sf::Vector2f(150, 200)); // absolute position
    wall.setTextureRect(sf::IntRect(0, 0, 60, 500));
    texture_wall.setRepeated(true);
    walls.push_back(wall);

    //wall_2;
    wall.setTexture(texture_wall);
    wall.setScale(1, 1);
    wall.setPosition(sf::Vector2f(10, 125)); // absolute position
    wall.setTextureRect(sf::IntRect(0, 0, 60, 400));
    texture_wall.setRepeated(true);
    walls.push_back(wall);

//    //wall_3;
//    wall.setTexture(texture_wall);
//    wall.setScale(0.6, 0.6);
//    wall.setPosition(sf::Vector2f(550, 170)); // absolute position
//    wall.setTextureRect(sf::IntRect(0, 0, 60, 500));
//    texture_wall.setRepeated(true);
//    walls.push_back(wall);

//    //wall_4;
//    wall.setTexture(texture_wall);
//    wall.setScale(0.6, 0.6);
//    wall.setPosition(sf::Vector2f(250, 250)); // absolute position
//    wall.setRotation(-90); // absolute angle
//    wall.setTextureRect(sf::IntRect(0, 0, 60, 250));
//    texture_wall.setRepeated(true);
//    walls.push_back(wall);

//    //wall_5;
//    wall.setTexture(texture_wall);
//    wall.setScale(0.6, 0.6);
//    wall.setPosition(sf::Vector2f(450, 350)); // absolute position
//    wall.setRotation(-90); // absolute angle
//    wall.setTextureRect(sf::IntRect(0, 0, 60, 350));
//    texture_wall.setRepeated(true);
//    walls.push_back(wall);

//    //wall_6;
//    wall.setTexture(texture_wall);
//    wall.setScale(0.6, 0.6);
//    wall.setPosition(sf::Vector2f(300, 450)); // absolute position
//    wall.setRotation(-90); // absolute angle
//    wall.setTextureRect(sf::IntRect(0, 0, 60, 250));
//    texture_wall.setRepeated(true);
//    walls.push_back(wall);

//    //wall_7;
//    wall.setTexture(texture_wall);
//    wall.setScale(0.6, 0.6);
//    wall.setPosition(sf::Vector2f(220, 100)); // absolute position
//    wall.setRotation(-90); // absolute angle
//    wall.setTextureRect(sf::IntRect(0, 0, 60, 500));
//    texture_wall.setRepeated(true);
//    walls.push_back(wall);

    for(auto &w : walls) {
        hero.add_wall_bounds(w.getGlobalBounds());
    }

    // run the program as long as the window is open



    while (window.isOpen()) {

        hero.setScale(1.5,1.5);

        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }



        hero.gravitation(elapsed);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){hero.jump();}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){hero.MoveInDirection(elapsed,sf::Keyboard::Right);}
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){hero.MoveInDirection(elapsed,sf::Keyboard::Left);}
        else{hero.setTextureRect(sf::IntRect(50, 0, 50, 37));}




        window.clear(sf::Color::Black);

        // draw everything here...

        window.draw(sky);
        window.draw(sea);
        window.draw(clouds);

        for(auto w: walls)
            window.draw(w);

        window.draw(hero);

        // end the current frame
        window.display();
    }

    return 0;
};
