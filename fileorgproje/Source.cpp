#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <Windows.h>

using namespace std;

// menu flagleri
constexpr int EXIT = -0xE;
constexpr int NEUTR = -0x1;

// menu switcher
int switcher = NEUTR;

// handles unbuffered input from keyboard for menu
auto getstroke() {
	HANDLE h;
	bool isflushed = false;
	for (;;) {
		h = GetStdHandle(STD_INPUT_HANDLE);
		
		this_thread::sleep_for(chrono::milliseconds(33));
		switcher = NEUTR;
		if (GetAsyncKeyState(VK_ESCAPE)) {
			switcher = EXIT;
			cout << "BYE" << endl;
		}

		if((WAIT_OBJECT_0 == WaitForSingleObject(h,0))){
			
			if (GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState(0x31)) {
				switcher = 1;
			}
			else if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(0x32)) {
				switcher = 2;
			}
			else if (GetAsyncKeyState(VK_NUMPAD3) || GetAsyncKeyState(0x33)) {
				switcher = 3;
			}
			else {
				
			}
			for (; (WAIT_OBJECT_0 == WaitForSingleObject(h, 0)); isflushed = FlushConsoleInputBuffer(h)) {
				
			}
		}
	}
}

class slot {
public:
	slot() {
		cout << "slot created: " <<slots<<endl;
		position = slots + 1;
		slots++;
	}
	char owner = 'n';
	int flakes = 0;
	int position;
	static int slots;
};

int slot::slots = 0;

class base {
public:
	int perspective(char player, int slot) {
		if(player == 'x'){
			return slot - 1;
		}
		if (player == 'y') {
			return 24 - slot;
		}
	}
	slot slots[28]; // [24] x oyuncusu [25] y oyuncusu için broken flakes [26] x oyuncusu beared flakes [27] y oyuncusu beared flakes
	char player = 'n';
	char nobody = 'n';
	char opponent = 'n';
	int turn = 0;
	int dice[2] = {0,0};
};
class renderframe : public base {
public:
	void render() {
		//oyun durumunun ekrana yazdýrýlmasý
		system("cls");

		render_buffer.clear();
		for (int x = 0;x<5;x++) {
			render_buffer.push_back("");
		}

		if (player == 'x') {
			for (int x = 13;x<25;x++) {
				cout << x << "\t";
			}
			cout << endl;
			for (int x = 13; x < 25; x++) {
				cout << "____" << "\t";
			}
			cout << endl;
		}
		else {
			for (int x = 12; x >0; x--) {
				cout << x << "\t";
			}
			cout << endl;
			for (int x = 12; x > 0; x--) {
				cout << "____" << "\t";
			}
			cout << endl;
		}

		for (int x = 12; x < 24; x++) {
			render_buffer.at(0) += "\\";
			render_buffer.at(0) += to_string(slots[x].flakes);
			render_buffer.at(0) += slots[x].owner;
			render_buffer.at(0) += "/";
			render_buffer.at(0) += "\t";
			render_buffer.at(1) += " ||\t";
			render_buffer.at(3) += " ||\t";
		}
		for (int x = 11; x >=0; x--) {
			render_buffer.at(4) += "/";
			render_buffer.at(4) += to_string(slots[x].flakes);
			render_buffer.at(4) += slots[x].owner;
			render_buffer.at(4) += "\\";
			render_buffer.at(4) += "\t";
		}
		for (int x = 0;x<4;x++) {
			render_buffer.at(2) += "\t";
		}
		render_buffer.at(2) += "<";
		render_buffer.at(2) += to_string(slots[24].flakes);
		render_buffer.at(2) += ">";
		render_buffer.at(2) += "\t";
		render_buffer.at(2) += "[";
		render_buffer.at(2) += to_string(dice[0]);
		render_buffer.at(2) += "]";
		render_buffer.at(2) += "\t";
		render_buffer.at(2) += "[";
		render_buffer.at(2) += to_string(dice[1]);
		render_buffer.at(2) += "]";
		render_buffer.at(2) += "\t";
		render_buffer.at(2) += "<";
		render_buffer.at(2) += to_string(slots[25].flakes);
		render_buffer.at(2) += ">";
		for (int x = 0; x < 4; x++) {
			render_buffer.at(2) += "\t";
		}

		for (auto &render:render_buffer) {
			cout << render<<endl;
		}
		if (player == 'y') {
			for (int x = 13; x < 25; x++) {
				cout << "----" <<"\t";
			}
			cout << endl;
			for (int x = 13; x < 25; x++) {
				cout << x << "\t";
			}
			cout << endl;
		}
		else {
			for (int x = 12; x >0; x--) {
				cout << "----" << "\t";
			}
			cout << endl;
			for (int x = 12; x > 0; x--) {
				cout << x << "\t";
			}
			cout << endl;
		}
	}
	void rendertofile() {
		// oyun durumunun dosyaya yazdýrýlmasý
		ofstream renderfile("Table.csv",ios::trunc);
		if (renderfile.is_open()) {
			for (int x = 12; x < 24; x++) {
				renderfile << to_string(slots[x].flakes);
				renderfile <<  slots[x].owner;
				renderfile << "\t";
			}
			renderfile << endl;
			for (int x = 12; x < 24; x++) {
				renderfile << "\t";
			}
			renderfile << endl;
			for (int x = 0; x < 4; x++) {
				renderfile << "\t";
			}
			renderfile << to_string(slots[24].flakes);
			renderfile << "\t";
			renderfile << to_string(dice[0]);
			renderfile << "\t";
			renderfile << to_string(dice[1]);
			renderfile << "\t";
			renderfile << to_string(slots[25].flakes);
			for (int x = 0; x < 4; x++) {
				renderfile << "\t";
			}
			renderfile << endl;
			for (int x = 12; x < 24; x++) {
				renderfile << "\t";
			}
			renderfile << endl;
			for (int x = 11; x >= 0; x--) {
				renderfile << to_string(slots[x].flakes);
				renderfile << slots[x].owner;
				renderfile << "\t";
			}
			renderfile.close();
		}
	}
	void getlog(int mode) {
		// log kayýtlarý mode == 0 olduðunda ekrana yazdýrýlýr mode == 1 olduðunda dosyadan okunur
		if (mode ==0) {
			for (auto &log:logs) {
				cout << log << endl;
			}
			logs.clear();
			system("pause");
		}
		else {
			ifstream readlog("Logs.txt");
			if (readlog.is_open()) {
				string log;
				while (getline(readlog,log)) {
					logs.push_back(log);
				}
				readlog.close();
			}
		}
	}
	void log() {
		// log kaydý alýnýyor
		logs.push_back("");
		logs.back() = string(1,player) + "\t" + to_string(dice[0]) + "\t" + to_string(dice[1]);
	}
	void logtofile() {
		// dosyaya log kayýtlarý yazdýrýlýyor
		ofstream writelog("Logs.txt",ios::app);
		if (writelog.is_open()) {
			writelog << logs.back() << endl;
			writelog.close();
		}
	}
	
	vector<string> logs;
	vector<string> render_buffer;
};

class game : public renderframe {
public:
	void newgame() {
		// log temizlenir
		logs.clear();
		ofstream clearlog("Logs.txt", ios::trunc);
		clearlog.close();

		// oyun baþlangýcý taþ dizilimi
		slots[perspective('x', 24)].flakes = 2;
		slots[perspective('x', 24)].owner = 'x';
		slots[perspective('x', 8)].flakes = 3;
		slots[perspective('x', 8)].owner = 'x';
		slots[perspective('x', 13)].flakes = 5;
		slots[perspective('x', 13)].owner = 'x';
		slots[perspective('x', 6)].flakes = 5;
		slots[perspective('x', 6)].owner = 'x';
		slots[perspective('y', 24)].flakes = 2;
		slots[perspective('y', 24)].owner = 'y';
		slots[perspective('y', 8)].flakes = 3;
		slots[perspective('y', 8)].owner = 'y';
		slots[perspective('y', 13)].flakes = 5;
		slots[perspective('y', 13)].owner = 'y';
		slots[perspective('y', 6)].flakes = 5;
		slots[perspective('y', 6)].owner = 'y';

		// oyun baþlangýcý zar atýmý
		for (;;) {
			dice[0] = rand() % 7;
			dice[1] = rand() % 7;
			if ((dice[0] != 0) && (dice[1] != 0) && dice[1] != dice[0]) {
				if (dice[0] > dice[1]) {
					player = 'x';
					break;
				}
				else {
					player = 'y';
					break;
				}
			}
		}
		logs.push_back("x\t");
		logs.back() += to_string(dice[0]);
		logtofile();
		logs.push_back("y\t");
		logs.back() += to_string(dice[1]);
		logtofile();
		cout << "x rolled " << dice[0] << endl << "y rolled " << dice[1] << endl << "Player " << player << " plays first." << endl;
		system("pause");
	}

	void savegame() {
		// oyunun kaydedilmesi
		
		ofstream save("Save.sav",ios::trunc);
		if (save.is_open()) {
			for (auto &slot: slots) {
				save << slot.flakes << ",";
			}
			save << endl;
			for (auto &slot:slots) {
				save << slot.owner << ",";
			}
			save << endl << player << "," << opponent;
			save.close();
		}
	}

	bool loadgame() {
		// oyunun dosyadan okunup yüklenmesi

		ifstream load("Save.sav");
		if (load.is_open()) {
			string line;
			string temp;

			if (!(load.peek() == ifstream::traits_type::eof())) {
				cout << "Loading" << endl;
			}
			else {
				cout << "Not available." << endl;
				system("pause");
				return false;
			}
			getline(load, line);
			for (auto &slot:slots) {
				temp = line.substr(0, line.find(","));
				slot.flakes = stoi(temp);
				line = line.substr(line.find(",") + 1, line.size());
			}
			getline(load, line);
			for (auto& slot : slots) {
				temp = line.substr(0, line.find(","));
				slot.owner = temp[0];
				line = line.substr(line.find(",") + 1, line.size());
			}
			getline(load, line);
			temp = line.substr(0, line.find(","));
			player = temp[0];
			line = line.substr(line.find(",") + 1, line.size());
			temp = line.substr(0, line.find(","));
			opponent = temp[0];

			load.close();
		}
		else { 
			cout << "Not available." << endl;
			system("pause");
			return false;
		}
		return true;
	}

	void gameturn() {
		// rakip belirleyici
		if (player == 'x') {
			opponent = 'y';
		}
		else if (player == 'y') {
			opponent = 'x';
		}

		// her tur zar atýmý
		for (;;) {
			dice[0] = rand() % 7;
			dice[1] = rand() % 7;
			if ((dice[0] != 0) && (dice[1] != 0)) {
				break;
			}
		}

		// hamle ölçümleri
		vector<vector<int>> moves;
		int movetoposition;
		int movefromposition;
		int positionchecker;

		// hamle seçimi
		int makemove = 0;
		int whichdice = 0;
		bool diceismoved[2] = {false,false};
		bool diceisdoubled[2] = { false,false };
		vector<int> selectmove;

		if (dice[0] != dice[1]) {
			diceisdoubled[0] = true;
			diceisdoubled[1] = true;
		}

		for (; diceismoved[0] == false || diceismoved[1] == false || diceisdoubled[0] == false || diceisdoubled[1] == false;) {

			render();
			selectmove.clear();
			moves.clear();

			// hamle ölçümleri
			bool hit = false;
			if (player == 'x' && slots[24].flakes != 0) {
				hit = true;
			}
			else if (player == 'y' && slots[25].flakes != 0) {
				hit = true;
			}
			for (auto a_dice : dice) {
				for (auto _slot : slots) {
					if (_slot.owner == player && !hit) {
						if (player == 'x') {
							movetoposition = perspective(player, _slot.position - a_dice) + 1;
							movefromposition = perspective(player, _slot.position) + 1;
						}
						else {
							movetoposition = perspective(player, _slot.position + a_dice) + 1;
							movefromposition = perspective(player, _slot.position) + 1;
						}
						positionchecker = perspective(player, movetoposition);
						if (0 < movetoposition && movetoposition < 25) {
							if (slots[positionchecker].owner == player) {
								moves.push_back({ movefromposition,movetoposition,a_dice });
								continue;
							}
							if (slots[positionchecker].owner != opponent) {
								moves.push_back({ movefromposition,movetoposition, 1,a_dice });
								continue;
							}
							else {
								if (slots[positionchecker].flakes == 1) {
									moves.push_back({ movefromposition,movetoposition, 0,a_dice });
									continue;
								}
							}
						}
						else {
							moves.push_back({ movefromposition,a_dice });
							continue;
						}
					}
					else if ((_slot.owner == nobody || _slot.owner == player) && hit){
						movetoposition = perspective(player, _slot.position) + 1;
						if (movetoposition == a_dice) {
							if (player == 'x') {
								moves.push_back({ 24 ,movetoposition,a_dice });
							}
							else {
								moves.push_back({ 25 ,movetoposition,a_dice });
							}
						}
					}
				}
				if (dice[0] == dice[1]) {
					break;
				}
			}

			if (diceismoved[0] == true && diceisdoubled[0] == false) {
				diceisdoubled[0] = true;
				diceismoved[0] = false;
			}
			if (diceismoved[1] == true && diceisdoubled[1] == false) {
				diceisdoubled[1] = true;
				diceismoved[1] = false;
			}

			cout << endl <<"Player "<<player<<"'s turn"<<endl<<endl<< "--Rolled dices-- \n1: " << dice[0];
			if (diceismoved[0] == true) {
				cout << " (moved)";
			}
			cout << "\n2: " << dice[1];
			if (diceismoved[1] == true) {
				cout << " (moved)";
			}
			cout << endl << "Choose a dice to make a move: ";
			
			cin >> whichdice;
			if (whichdice != 2 && whichdice != 1) {
				whichdice = 1;
			}
			whichdice = whichdice - 1;

			
			if (diceismoved[whichdice] == true && whichdice == 1) {
				whichdice = 0;
			}
			else if (diceismoved[whichdice] == true && whichdice == 0) {
				whichdice = 1;
			}

			// hamle seçimi
			int order = 1;
			for (int x = 0; x < moves.size(); x++) {
				if (moves.at(x).size() == 4 && moves.at(x).at(3) == dice[whichdice]) {
					if (moves.at(x).at(2) == 0) {
						cout << order << ". Hit position (" << moves.at(x).at(0) << ")->("<< moves.at(x).at(1) <<") for dice [" << moves.at(x).back() << "]." << endl;
						order++;
						selectmove.push_back(x);
					}
					if (moves.at(x).at(2) == 1) {
						cout << order << ". Capture position (" << moves.at(x).at(0) << ")->(" << moves.at(x).at(1) << ") for dice [" << moves.at(x).back() << "]." << endl;
						order++;
						selectmove.push_back(x);
					}
				}
				else if (moves.at(x).size() == 3 && moves.at(x).at(2) == dice[whichdice] && !hit) {
					cout << order << ". Move to position (" << moves.at(x).at(0) << ")->(" << moves.at(x).at(1) << ") for dice [" << moves.at(x).back() << "]." << endl;
					order++;
					selectmove.push_back(x);
				}
				else if (moves.at(x).size() == 2 && moves.at(x).at(1) == dice[whichdice]) {
					cout << order << ". Bearing off position (" << moves.at(x).at(0) << ") for dice [" << moves.at(x).back() << "]." << endl;
					order++;
					selectmove.push_back(x);
				}
				else if (hit == true && moves.at(x).at(2) == dice[whichdice]) {
					cout << order << ". Enter to position (" << moves.at(x).at(1) << ") for dice [" << moves.at(x).back() << "]." << endl;
					order++;
					selectmove.push_back(x);
				}
			}

			cout << "0: Back"<<endl<<endl << "Select: "; cin >> makemove;
			if (selectmove.empty()) {
				diceismoved[whichdice] = true;
				cout << "Cannot make a move."<<endl;
				system("pause");
			}
			
			if (makemove <= 0 || makemove > selectmove.size()) {
				render();
				continue;
			}
			else {
				makemove = makemove - 1;
				makemove = selectmove.at(makemove);
				if (moves.at(makemove).size() == 4) {
					if (moves.at(makemove).at(2) == 0) {
						slots[perspective(player, (moves.at(makemove).at(0)))].flakes -= 1;
						slots[perspective(player, (moves.at(makemove).at(1)))].owner = player;
						if (player == 'x') {
							slots[25].flakes++;
						}
						else {
							slots[24].flakes++;
						}
					}
					if (moves.at(makemove).at(2) == 1) {
						slots[perspective(player, (moves.at(makemove).at(0)))].flakes -= 1;
						slots[perspective(player, (moves.at(makemove).at(1)))].flakes += 1;
						slots[perspective(player, (moves.at(makemove).at(1)))].owner = player;
					}
				}
				else if (moves.at(makemove).size() == 3 && !hit) {
					slots[perspective(player, (moves.at(makemove).at(0)))].flakes -= 1;
					slots[perspective(player, (moves.at(makemove).at(1)))].flakes += 1;
				}
				else if (moves.at(makemove).size() == 2) {
					slots[perspective(player, (moves.at(makemove).at(0)))].flakes -= 1;
					if(player == 'x'){
						slots[26].flakes++;
					}
					else {
						slots[27].flakes++;
					}
				}
				else if (hit) {
					slots[moves.at(makemove).at(0)].flakes--;
					slots[perspective(player, (moves.at(makemove).at(1)))].flakes++;
					slots[moves.at(makemove).at(1)].owner = player;
				}
				diceismoved[whichdice] = true;
				for (auto &_slot : slots) {
					if (_slot.flakes == 0) {
						_slot.owner = nobody;
					}
				}
				if (player == 'x' && slots[26].flakes == 15) {
					cout << endl << "Player x wins."<<endl;
					endgame = true;
					return;
				}
				else if(player == 'y' && slots[27].flakes == 15){
					cout << endl << "Player y wins."<<endl;
					endgame = true;
					return;
				}
			}
			render();
		}

		// oyun durumu dosyaya ve ekrana yazdýrýlýyor
		render();
		rendertofile();

		// her tur oyuncu deðiþimi
		if (player == 'x') {
			player = 'y';
		}
		else if (player == 'y') {
			player = 'x';
		}

		// save dosyasý oluþturuluyor
		savegame();
		cout << "Player " << player << "'s turn passed. Game saved." << endl;
		system("pause");
	}

	bool endgame = false;
};

int main() 
{	
	thread unbuffered(getstroke);
	unbuffered.detach();

	srand(time(nullptr));
	game mygame;
	
	// main program loop
	for (;switcher !=EXIT;) {
		// main menu
		system("cls");
		cout << "1. New Game" << endl << "2. Load Game" << endl << "3. Read Log" << endl << "ESC. Exit" << endl << flush;
		for (;switcher !=EXIT;) {

			this_thread::sleep_for(chrono::milliseconds(16));

			if (switcher == 1) {
				// yeni oyun baþlatýlýr
				mygame.newgame();
				for (; !mygame.endgame;) {
					// oyun döngüsü
					mygame.gameturn();
					mygame.log();
					mygame.logtofile();
				}
				break;
			}
			else if (switcher == 2) {
				// var olan oyun yüklenir ve log kayýtlarý okunur
				if (mygame.loadgame()) {
					mygame.getlog(1);
					for (; !mygame.endgame;) {
						// oyun döngüsü
						mygame.gameturn();
						mygame.log();
						mygame.logtofile();
					}
				}
				break;
			}
			else if (switcher == 3) {
				// log kayýtlarý okunur
				mygame.getlog(1);
				mygame.getlog(0);
				break;
			}
		}
	}
	
	return 0;
}