// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std::chrono_literals; // Permet de faire des op�ration de temps avec s, min, h, ...

static const sf::Color Orange(255, 165, 0); // Cr�e la couleur orange � partir des valeurs RGB

const auto time_transit = 3s; 
const auto time_waiting = 8s;

enum class Traffic_color // Enum�ration de couleurs stock� dans la class Traffic_color. Cette d�claration permet d'utiliser directement le nom des couleurs 
{
    green = 0,
    orange = 1,
    red = 2
};

Traffic_color operator++(Traffic_color &traffic_color) // Surcharge de l'op�rateur ++ pour la class Traffic_color
{
    switch (traffic_color)
    {
    case Traffic_color::red: // Si la couleur actuelle est rouge, elle devient verte
        traffic_color = Traffic_color::green;
        break;

    case Traffic_color::green: // Si la couleur actelle est verte
        std::this_thread::sleep_for(time_transit); // Attendre 3 secondes
        traffic_color = Traffic_color::orange; // La couleur devient orange
        std::this_thread::sleep_for(time_transit); // Attendre 3 secondes
        traffic_color = Traffic_color::red; // La couleur devient rouge
    }
    return traffic_color; // On retourne la couleur actuelle
}

class Traffic_light // Cr�ation de la class Traffic_light
{
private:
    Traffic_color traffic_color_; // variable correspondant � une �num�ration de couleurs : vert, orange et rouge

public:
    explicit Traffic_light(const Traffic_color &traffic_color) : traffic_color_{traffic_color} {} // Constructeur qui prend un Traffic_color en param�tre
    void operator++() // Surcharge de l'op�rateur ++ pour la classe Traffic_light 
    {
        ++traffic_color_;
    }
    void set_traffic_color(const Traffic_color &traffic_color) // Fonction qui renvoie
    {
        traffic_color_ = traffic_color;
    }
    const Traffic_color &get_traffic_color() const // Fonction qui renvoie la couleur actuelle du feu tricolore
    {
        return traffic_color_;
    }
};

const sf::Color &get_SFML_color(const Traffic_light &traffic_light) // Fonction qui retourne la couleur actuelle du feu tricolore dans la class Color
{
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return Orange;
}

std::ostream &operator<<(std::ostream &os, const Traffic_light &traffic_light) // Surcharge de l'op�rateur << (pour print) quand la class Traffic_light
{                                                                              // est donn�e afin de retourner un simple string
    std::string ptr;
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::red:
        ptr = "Red";
        break;
    case Traffic_color::green:
        ptr = "Green";
        break;
    case Traffic_color::orange:
        ptr = "Orange";
    }
    os << ptr;
    return os;
}

void run_traffic_light(Traffic_light &traffic_light_master, Traffic_light &traffic_light_slave, std::stop_token stop_token)
{
    traffic_light_master.set_traffic_color(Traffic_color::green); // Donne la couleur de d�part verte au feu tricolore ma�tre
    traffic_light_slave.set_traffic_color(Traffic_color::red);    // Donne la couleur de d�part rouge au feu tricolore esclave
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(time_waiting);
        if (traffic_light_master.get_traffic_color() == Traffic_color::green) // Si la couleur du feu tricolore m�itre est verte
        {
            ++traffic_light_master; // Fait passer le feu tricolore ma�tre au orange puis au rouge
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au vert une fois que le feu tricolore ma�tre est pass� au rouge
        }
        else
        {
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au orange puis au rouge
            ++traffic_light_master; // Fait passer le feu tricolore ma�tre au vert une foisque le feu tricolore esclave est pass� au rouge
        }
    }
}

void print_traffic_light(Traffic_light &traffic_light_master, Traffic_light &traffic_light_slave, std::stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(1s);
        std::cout << "Taffic light master : " << traffic_light_master << " Taffic light slave : " << traffic_light_slave << std::endl;
    }
}

int main()
{
    std::stop_source stopping; // Cr�e stopping de la classe stop_source. Cela permet de g�n�rer de requ�tes d'arr�ts 
    Traffic_light traffic_light_master{Traffic_color::red}; // Cr�e le feu tricolore ma�tre est esclave et les initialise
    Traffic_light traffic_light_slave{Traffic_color::red};  // avec la couleur rouge par d�faut
    std::jthread thread_traffic_light_master(run_traffic_light,
                                             std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
                                     std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(800, 800), "My window"); // Cr�e une fen�tre "My window" de dessin 2D SFML de 800 x 800 pixels 

    float l1 = 350, l2 = 430, size = 800, radius = 10;
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(size, l1))}; 
    // Cr�e une ligne s'appelant line1 qui commence au point (0, 350) et se termine au point (800, 350) de couleur blanc (par d�faut)  
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(0, l2)), sf::Vertex(sf::Vector2f(size, l2))};
    sf::Vertex line3[] = {sf::Vertex(sf::Vector2f(l1, 0)), sf::Vertex(sf::Vector2f(l1, size))};
    sf::Vertex line4[] = {sf::Vertex(sf::Vector2f(l2, 0)), sf::Vertex(sf::Vector2f(l2, size))};

    sf::CircleShape circle1(radius); // Cr�e un cercle s'appelant cercle1 de rayon 10 pixels
    circle1.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2); // D�finit l'origine du cercle
    circle1.setPosition(l2 + radius / 2, l2 + radius / 2); // D�finit la position du cercle dans la fen�tre
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(l2 + radius / 2, l1 - radius);

    while (window.isOpen()) // Tant que la fen�tre est ouverte
    {
        sf::Event event; // Cr�e un event qui permet de stocker des �v�nements en attente
        while (window.pollEvent(event)) // Tant qu'il y a des �v�nements en attente dans la file d'attente de �v�nements de la fen�tre
        {
            if (event.type == sf::Event::Closed) //  Si l'�v�nement est la fermeture de la fen�tre
            {
                stopping.request_stop(); // Envoie une requ�te d'arr�t � tous les stop_token cr��s
                window.close(); // Ferme la fen�tre
                return 0;
            }
        }
        window.clear(sf::Color::Black); // Clear la fen�tre et affiche du noir

        window.draw(line1, 2, sf::Lines); // Dessine la line1
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);
        circle1.setFillColor(get_SFML_color(traffic_light_slave)); // Change la couleur de fond du cercle1
        circle2.setFillColor(get_SFML_color(traffic_light_master));
        window.draw(circle1); // Dessine le cercle1
        window.draw(circle2);

        window.display(); // Affiche la fen�tre
    }

    return 0;
}