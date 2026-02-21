#include "../includes/Server.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cout << "error messages ..." << std::endl;
        exit(1);
    }
    // yel-bouz parse the input here av[1] port (should be int), av[2] password (should be string) if there is an error exit
    try
    {
        Server server(std::atoi(av[1]), av[2]);
        server.run();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        // we should clean and return 1
    }
    

}