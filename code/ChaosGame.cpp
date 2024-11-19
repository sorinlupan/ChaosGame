#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <time.h>

using namespace sf;
using namespace std;

class Fractal                                                                        //abstract class Fractal definition
{
public:
    virtual void addVertex(const Vector2f& vertex) = 0;
    virtual void generateCorners() = 0;
    virtual void draw(RenderWindow& window) = 0;
    virtual ~Fractal() = default;
};

class TriangleFractal : public Fractal                                               //derived class TriangleFractal definition
{
public:
    void addVertex(const Vector2f& vertex) override                                  
    {
        if (vertices.size() < 3) { vertices.push_back(vertex); }
        else if (points.empty()) { points.push_back(vertex); }
    }

    void generateCorners() override
    {
        if (!points.empty() && vertices.size() == 3)
        {
            for (int i = 0; i < 300; i++)
            {
                int randomIndex = rand() % 3;
                Vector2f randomVertex = vertices[randomIndex];
                Vector2f lastPoint = points.back();
                Vector2f newPoint((lastPoint.x + randomVertex.x) / 2, (lastPoint.y + randomVertex.y) / 2);
                points.push_back(newPoint);
            }
        }
    }

    void draw(RenderWindow& window) override
    {
        for (const auto& vertex : vertices)            //drawing vertices
        {
            RectangleShape rect(Vector2f(10, 10));
            rect.setPosition(vertex);
            rect.setFillColor(Color::White);
            window.draw(rect);
        }
        for (const auto& point : points)               //drawin
        {
            RectangleShape dot(Vector2f(2, 2));
            dot.setPosition(point);
            dot.setFillColor(Color::Blue);
            window.draw(dot);
        }
    }
private:
    vector<Vector2f> vertices;
    vector<Vector2f> points;
};

class SquareFractal : public Fractal												//derived class SquareFractal definition
{
public:
    void addVertex(const Vector2f& vertex) override
    {
        if (vertices.size() < 4) { vertices.push_back(vertex); }
        else if (points.empty()) { points.push_back(vertex); }
    }

    void generateCorners() override
    {
        if (!points.empty() && vertices.size() == 4)
        {
            for (int i = 0; i < 400; i++)
            {
                int randomIndex = rand() % 4;
                Vector2f randomVertex = vertices[randomIndex];
                Vector2f lastPoint = points.back();
                Vector2f newPoint((lastPoint.x + randomVertex.x) / 2, (lastPoint.y + randomVertex.y) / 2);
                points.push_back(newPoint);
            }
        }
    }

    void draw(RenderWindow& window) override
    {
        for (const auto& vertex : vertices)
        {
            RectangleShape sqr(Vector2f(10, 10));
            sqr.setPosition(vertex);
            sqr.setFillColor(Color::White);
            window.draw(sqr);
        }
        for (const auto& point : points)
        {
            RectangleShape dot(Vector2f(2, 2));
            dot.setPosition(point);
            dot.setFillColor(Color::Red);
            window.draw(dot);
        }
    }
private:
    vector<Vector2f> vertices;
    vector<Vector2f> points;
};


int main()                                                                                      /// M  A  I  N  ///
{
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Chaos Game", Style::Default);
    
    sf::Font font;                                                                              //start font load
    try
    {
        if (!font.loadFromFile("botched.ttf")) { throw runtime_error("Font not loaded."); }
    }
    catch (const exception& rte) { cout << rte.what() << endl; }                                //end font load

    sf::Text title, dir1, dir2;                                                                 //create text objects
    title.setFont(font);
    dir1.setFont(font);
    dir2.setFont(font);
    title.setString("fractals.");
    dir1.setString("t for triangle. s for square.");
    dir2.setString("when you are satisfied, press escape to quit.");
    title.setCharacterSize(250);
    dir1.setCharacterSize(50);
    dir2.setCharacterSize(50);
    title.setFillColor(sf::Color{ 255, 255, 255, 200 });
    dir1.setFillColor(sf::Color{ 255, 255, 255, 50 });
    dir2.setFillColor(sf::Color{ 255, 255, 255, 50 });
    title.setPosition(1050, 880);
    dir1.setPosition(50, 30);
    dir2.setPosition(50, 65);

    Fractal* fractal = nullptr;
    bool fractalSelected = false;
    srand(time(0));                             

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) { window.close(); }

            if (event.type == Event::KeyPressed && !fractalSelected)
            {
                if (event.key.code == Keyboard::T)
                {
                    fractal = new TriangleFractal();
                    fractalSelected = true;
                    cout << "Triangle fractal selected." << endl;
                }
                else if (event.key.code == Keyboard::S)
                {
                    fractal = new SquareFractal();
                    fractalSelected = true;
                    cout << "Square fractal selected." << endl;
                }
            }
            if (event.type == Event::MouseButtonPressed && fractalSelected)
            {
                if (event.mouseButton.button == Mouse::Left) { fractal->addVertex(Vector2f(event.mouseButton.x, event.mouseButton.y)); }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); }
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            window.clear();
            delete fractal;
            fractalSelected = false;
            cout << "Restarting..." << endl;
        }
        if (fractalSelected) { fractal->generateCorners(); }

        window.clear();
        
        window.draw(title);
        window.draw(dir1);
        window.draw(dir2);

        if (fractal) { fractal->draw(window); }
        
        window.display();

    }

    delete fractal;
    return 0;
}
