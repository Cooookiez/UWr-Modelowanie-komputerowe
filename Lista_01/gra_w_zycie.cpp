#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <unistd.h>
#include <string>

#define RESET   "\033[0m"
#define CLEAR   RESET
#define BLACK   "\033[40m"
#define RED     "\033[41m"
#define WHITE   "\033[47m"
#define BORDER  "[]"
#define CELL    "  "

using namespace std;

string replaceChar(string str, char ch1, char ch2) {
    // replace ch1 to ch2
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ch1) str[i] = ch2;
    }
    return str;
}

class GameOfLife {
private:

    double p;
    int h, w;
    vector< vector<bool> > cells_main;
    vector< vector<bool> > cells_temp;

    void clearConsole() {
		system("clear");
		cout << "\033[0;0H\033[0m";
    }

    void drawBoard() {
        for (int i = 0; i < h+2; i++) {
            // first & last line
            if(i == 0 || i == h+1) {
                cout << RED << endl;
                for (int j = 0; j < w+2; j++) {
                    cout << "\033[" << 1+i << ";"<< 1+j*2 << "H" << BORDER;
                }
            // right and left line
            } else {
                for (int j = 0; j < w+2; j++) {
                    if (j == 0 || j == w+1) {
                        cout << RED;
                        cout << "\033[" << 1+i << ";"<< 1+j*2 << "H" << BORDER;
                    } else {
                        cout << BLACK;
                        cout << "\033[" << 1+i << ";"<< 1+j*2 << "H" << CELL;
                    }
                }
            }
            cout << CLEAR << endl;
        }
    }

    void drawCells() {
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (cells_main[i][j] == 1) {
                    cout << WHITE << endl;
                } else {
                    cout << BLACK << endl;
                }
                cout << "\033[" << 2+i << ";"<< 1+2+j*2 << "H" << CELL;
            }
            cout << CLEAR << endl;
        }
    }

    void generateCells() {
        // random
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);

        cells_main.clear(); // clear main vector
        vector<bool> tmpVecCells;
        for (int i = 0; i < h; i++) {
            tmpVecCells.clear();
            // generate i=one line and add to main vector
            for (int j = 0; j < w; j++) {
                tmpVecCells.push_back(dis(gen) < p);
            }
            cells_main.push_back(tmpVecCells);
        }
    }

    int numberOfNeighbors(int h, int w) {
        int numbers = 0;
        int cell_up = h-1 >= 0 ? h-1 : this->h-1;
        int cell_down = h+1 < this->h ? h+1 : 0;
        int cell_right = w+1 < this->w ? w+1 : 0;
        int cell_left = w-1 >= 0 ? w-1 : this->w-1;

        // 1 2 3
        // 4 _ 6
        // 7 8 9

        numbers += cells_main[cell_up][cell_left];      // 1
        numbers += cells_main[cell_up][w];              // 2
        numbers += cells_main[cell_up][cell_right];     // 3
        numbers += cells_main[h][cell_left];            // 4
        numbers += cells_main[h][cell_right];           // 6
        numbers += cells_main[cell_down][cell_left];    // 7
        numbers += cells_main[cell_down][w];            // 8
        numbers += cells_main[cell_down][cell_right];   // 9
        return numbers;
    }

    void generateNextStep() {
        cells_temp = cells_main;
        int numb_of_n;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                numb_of_n = numberOfNeighbors(i, j);
                switch (cells_main[i][j]) {
                    // i was alive, but ...
                    case 1:
                        switch (numb_of_n) {
                            case 0: // im lonly
                            case 1: // me to
                            case 4: // die!
                            case 5: // die!
                            case 6: // die!
                            case 7: // die!
                            case 8: // die!
                                cells_temp[i][j] = 0;
                                break;
                            
                            case 2: // dont warry, im fine
                            case 3: // im hardly
                                cells_temp[i][j] = 1;
                                break;
                        }
                        break;

                    // i was dead, but ...
                    case 0:
                        switch (numb_of_n) {
                            case 1: // and nothing changed
                            case 2: // and nothing changed
                            case 4: // and nothing changed
                            case 5: // and nothing changed
                            case 6: // and nothing changed
                            case 7: // and nothing changed
                            case 8: // and nothing changed
                                cells_temp[i][j] = 0;
                                break;
                            
                            case 3: // but i did reborn
                                cells_temp[i][j] = 1;
                                break;
                        }
                        break;
                }
            }
        }
        cells_main = cells_temp;
    }

public:
    GameOfLife() {
        this->p = 0.5;
        this->h = 100;
        this->w = 100;
        generateCells();
    }
    GameOfLife(double p, int h) {
        this->p = p;
        this->h = h;
        this->w = h;
        generateCells();
    }
    GameOfLife(double p, int h, int w) {
        this->p = p;
        this->h = h;
        this->w = w;
        generateCells();
    }
    
    void draw() {
        this->clearConsole();
        this->drawBoard();
        this->drawCells();
    }

    void nextStep() {
        generateNextStep();
    }

    vector< vector<bool> > getCells() {
        return cells_main;
    }

    double getPopulation() {
        int noOfLife = 0;
        int noOfCells = 0;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                noOfCells++; // <- addin is bettter then times?
                if (cells_main[i][j])
                    noOfLife++;
            }
        }
        return (double)noOfLife / (double)noOfCells;
    }

};

int main(int argc, char* argv[]) {

    GameOfLife myGame;

    double p;
    int h, w;

    switch (argc) {

        case 2:
            if (strcmp(argv[1], "zad1") == 0) {
                myGame = GameOfLife(.5, 100);
                while (true) {
                    myGame.draw();
                    myGame.nextStep();
                    usleep(5000 * 100);
                }
                break;
            } else if (strcmp(argv[1], "zad2") == 0) {
                int t_max = 1200;
                double p[] = {.05, .10, .15, .20, .25, .30, .35, .40, .45, .50, .55, .60, .65, .70, .75, .80, .85, .90, .95};
                int p_size = sizeof(p)/sizeof(double);
                double values[p_size][t_max];
                for (int i = 0; i < p_size; i++) {
                    myGame = GameOfLife(p[i], 100);
                    int percentPrev = 0;
                    int percentCur = 0;
                    for (int t = 0; t < t_max; t++) {
                        percentCur = round((double)t / t_max * 100);
                        if (percentCur != percentPrev) {
                            cout << "p: " << p[i] << "\t" << percentCur << " percent" << endl;
                            percentPrev = percentCur;
                        }
                        values[i][t] = myGame.getPopulation();
                        myGame.nextStep();
                    }
                    cout << endl;
                }
                // save to file
                ofstream ofile;
                string filename = "data/zad2.txt";
                ofile.open(filename, ios::out);
                string tmpVal;
                //header
                // ofile << "t";
                // for (int i = 0; i < p_size; i++)
                //     ofile << "\t" << p[i];
                // ofile << endl;
                // values
                for (int i = 0; i < t_max; i++) {
                    ofile << i; // t (step)
                    for (int j = 0; j < p_size; j++) {
                        tmpVal = to_string(values[j][i]);
                        tmpVal = replaceChar(tmpVal, '.', ',');
                        ofile << "\t" << tmpVal;
                    }
                    ofile << endl;
                }
                ofile.close();
            }
            else if (strcmp(argv[1], "zad3") == 0) {
                ofstream ofile;
                int n_max = 100;
                int t_max = 1000;
                double p = .75; //p0
                double L[] = {10, 100, 200, 500, 1000}; // wielkosc ukłądu (m x n)
                for (int l = 0; l < sizeof(L)/sizeof(double); l++) {
                    myGame = GameOfLife(p, L[l]);
                    for (int n = 0; n < n_max; n++) {
                        string filename = "data/zad3_L_equal_to_" + to_string(L[l]) + ".txt";
                        ofile.open(filename, ios::in);
                        for (int t = 0; t < t_max; t++) {
                            
                        }
                    }
                }
            }
    }

    return 0;
}