#include "Conway.hpp"



int main()
{
    ConwaySettings settings(100, 100, 8);
    
    Conway conway(settings);
    conway.run();

    return 0;
}