#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// Forgatási irányok térképe
map<string, string> rotationMap = {
    {"N", "E"}, {"E", "S"}, {"S", "W"}, {"W", "N"}
};

// Forrás struktúra
struct Source {
    string id;
    string color;
    int x, y;
    vector<string> outputs;
};

// Fogyasztó struktúra
struct Consumer {
    string id;
    string color;
    int x, y;
    vector<string> inputs;
    bool flooded = false;
};

// Cső struktúra
struct Pipe {
    string id;
    int x = -1, y = -1; // A cső kezdetben nincs a pályán
    vector<string> connections;

    // Forgatás 90 fokkal
    void rotate() {
        for (auto &dir : connections) {
            dir = rotationMap[dir];
        }
    }

    // Ellenőrzi, hogy a cső illeszkedik-e az adott irányokhoz
    bool fits(string from, string to) {
        return (connections[0] == from && connections[1] == to) ||
               (connections[0] == to && connections[1] == from);
    }
};

// Pálya méret
struct Board {
    int rows, cols;
};

// Beolvasási funkciók
void readBoard(ifstream &file, Board &board) {
    file >> board.rows >> board.cols;
}

void readSources(ifstream &file, vector<Source> &sources) {
    int count;
    file >> count;
    for (int i = 0; i < count; ++i) {
        Source source;
        file >> source.id >> source.color >> source.x >> source.y;
        int outputCount;
        file >> outputCount;
        for (int j = 0; j < outputCount; ++j) {
            string output;
            file >> output;
            source.outputs.push_back(output);
        }
        sources.push_back(source);
    }
}

void readConsumers(ifstream &file, vector<Consumer> &consumers) {
    int count;
    file >> count;
    for (int i = 0; i < count; ++i) {
        Consumer consumer;
        file >> consumer.id >> consumer.color >> consumer.x >> consumer.y;
        int inputCount;
        file >> inputCount;
        for (int j = 0; j < inputCount; ++j) {
            string input;
            file >> input;
            consumer.inputs.push_back(input);
        }
        consumers.push_back(consumer);
    }
}

void readPipes(ifstream &file, vector<Pipe> &pipes) {
    int count;
    file >> count;
    for (int i = 0; i < count; ++i) {
        Pipe pipe;
        file >> pipe.id;
        int connectionCount;
        file >> connectionCount;
        for (int j = 0; j < connectionCount; ++j) {
            string conn;
            file >> conn;
            pipe.connections.push_back(conn);
        }
        pipes.push_back(pipe);
    }
}

void readPhases(ifstream &file, vector<string> &phases) {
    int count;
    file >> count;
    for (int i = 0; i < count; ++i) {
        string phase;
        file >> phase;
        phases.push_back(phase);
    }
}

// Megoldási algoritmus
void solve(const Board &board, vector<Source> &sources, vector<Consumer> &consumers, vector<Pipe> &pipes, vector<string> &phases) {
    cout << "Pályaméret: " << board.rows << "x" << board.cols << "\n";
    for (const auto &phase : phases) {
        cout << "Fázis: " << phase << "\n";
            for (auto &source : sources) {
            if (source.color == phase) {
                for (auto &consumer : consumers) {
                    if (consumer.color == phase) {
                        cout << "Forrás (" << source.x << ", " << source.y << ") összekötése "
                             << "fogyasztóval (" << consumer.x << ", " << consumer.y << ")\n";

                            // Koordináták alapján csövek elhelyezése
                            int cx1 = source.x + 1, cy1 = source.y;     // Első cső helye
                        int cx2 = source.x + 1, cy2 = source.y + 1; // Második cső helye

                        // Forgatások az első csőhöz (W, S irány)
                        for (auto &pipe : pipes) {
                            while (!pipe.fits("W", "S")) {
                                pipe.rotate();
                            }
                            pipe.x = cx1;
                            pipe.y = cy1;
                            cout << "Első cső elhelyezve (" << pipe.x << ", " << pipe.y
                                 << ") irányok: " << pipe.connections[0] << ", " << pipe.connections[1] << "\n";
                                break;
                        }

                        // Forgatások a második csőhöz (N, E irány)
                        for (auto &pipe : pipes) {
                            while (!pipe.fits("N", "E")) {
                                pipe.rotate();
                            }
                            pipe.x = cx2;
                            pipe.y = cy2;
                            cout << "Második cső elhelyezve (" << pipe.x << ", " << pipe.y
                                 << ") irányok: " << pipe.connections[0] << ", " << pipe.connections[1] << "\n";
                                break;
                        }

                        consumer.flooded = true;
                        cout << "Fogyasztó elárasztva (" << consumer.x << ", " << consumer.y << ")\n";
                    }
                }
            }
        }
    }
}

int main() {
    // Adatok tárolása
    Board board;
    vector<Source> sources;
    vector<Consumer> consumers;
    vector<Pipe> pipes;
    vector<string> phases;

    // Bemenet beolvasása
    ifstream file("csovek.txt");
    if (!file.is_open()) {
        cerr << "Nem sikerült megnyitni az input fájlt!\n";
        return 1;
    }

    string section;
    while (file >> section) {
        if (section == "[Board]") {
            readBoard(file, board);
        } else if (section == "[Sources]") {
            readSources(file, sources);
        } else if (section == "[Consumers]") {
            readConsumers(file, consumers);
        } else if (section == "[Pipes]") {
            readPipes(file, pipes);
        } else if (section == "[Phases]") {
            readPhases(file, phases);
        }
    }
    file.close();

    // Megoldás futtatása
    solve(board, sources, consumers, pipes, phases);

    return 0;
}
