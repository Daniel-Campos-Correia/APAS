#include <iostream>
#include <ctype.h>
#include <sys/types.h>
/*#include <sys/stat.h>
#include <unistd.h>
#include "include/gnuplot_i.hpp"//Gnuplot class handles POSIX-Pipe-communikation with Gnuplot
#include "include/graphics.h"
#include <iostream>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <conio.h>   //for getch(), needed in wait_for_key()
#include <windows.h> //for Sleep()
void sleep(int i)
{
    Sleep(i*1000);
}
#endif


#define SLEEP_LGTH 2  // sleep time in seconds
#define NPOINTS    10// length of array

void wait_for_key(); // Programm halts until keypress

using std::cout;
using std::endl;
using namespace std;
extern Gnuplot g1("lines");
extern Gnuplot g2("lines");

int plotGrafics(int number, vector<string> xValue, vector<string> yValue, vector<string> x2Value, vector<string> y2Value, string legendX, string legendY1, string legendY2, string title)
{
    // if path-variable for gnuplot is not set, do it with:
    // Gnuplot::set_GNUPlotPath("C:/program files/gnuplot/bin/");

    #if defined(__APPLE__)
      // set a special standard terminal for showonscreen (normally not needed),
      //   e.g. Mac users who want to use x11 instead of aqua terminal:
     Gnuplot::set_terminal_std("x11");
    #endif
    Gnuplot g3("lines");
    if(number == 1){
        g3 = *g1;
    }
    else if(number == 2){
        g3 = *g2;
    }
    try
    {
        std::vector<double> x, y, y2, dy, z;

        for (unsigned int i = 0; i < NPOINTS; i++)  // fill double arrays x, y, z
        {
            x.push_back((double)i);             // x[i] = i
            y.push_back((double)i *2 + 2); // y[i] = i^2
            z.push_back( (double)i +1 );           // z[i] = x[i]*y[i] = i^3
        }

        g3.reset_all();
        g3.reset_plot();
        g3.set_grid();
        g3.set_xlabel(legendX);
        g3.set_ylabel(legendY1, legendY2);
        g3.set_title("title");
        g3.set_style("lines").plot_xy(x,y,legendY1,1).plot_xy(x,z,legendY2,2);
        g3.showonscreen(); // window output
        wait_for_key();
    }
    catch (GnuplotException &ge)
    {
        cout << ge.what() << endl;
    }
}



void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
*/
