#include<iostream>
#include<fstream>
using namespace std;

class ElevatorRequest {
public:
    int destination;
    int source;
    int ppl;
	int requiredMoves;
    void toString() {
        cout<<"ELEVATOR REQUEST -->> CURRENT FLOOR: "<<source<<" DESTINATION: "<<destination<<" PEOPLE: "<<ppl<<endl;
        cout<<endl;
    }
};

class Elevator {
public:
    ElevatorRequest eReq;
    Elevator() {
        eReq.source=0;
        eReq.destination=0;
        eReq.ppl=0;
		eReq.requiredMoves = 0;
    }
    int process(int id) {
    cout<<endl<<" ############## Moving ELEVATOR "<<(id+1)<<" FROM "<<eReq.source<<" TO "<< eReq.destination<<" ############## "<<endl
		<< "Waiting time: " << abs((eReq.source - eReq.destination)) << " moves.\n";
        eReq.source=eReq.destination;
        eReq.ppl=0;
		return abs((eReq.source - eReq.destination));
    }
    friend void toString(int, Elevator);
};
void toString(int id, Elevator elev) {
    cout<<" "<<(id+1)<<(elev.eReq.source==elev.eReq.destination?" @ REST":" @ MOVING")<<" SOURCE:"<<elev.eReq.source<<" DESTINATION:"<<elev.eReq.destination<<" PEOPLE:"<<elev.eReq.ppl<<endl;
}

class Elevator_Controller {
public:
    Elevator elevatorGroup[4]; //creates a group of 4 elevators
    void showElevatorStatus() {//print display of each elevator status
        cout<<"------------------------------------------------"<<endl;
        cout<<"ALL ELEVATOR STATUS"<<endl;
        for(int j=0;j<4;j++) {
            toString(j, elevatorGroup[j]);
        }
        cout<<"------------------------------------------------"<<endl;
        cout<<endl;
    }
    void process(ElevatorRequest currentRequest) {
        cout<<"BEFORE"<<endl;
        showElevatorStatus();
        cout<<"PROCESSING ELEVATOR REQUEST........."<<endl;
        currentRequest.toString();
        int found = 0;
        do {
            for(int i=0;i<4;i++) {
                // IF IN REST and DESTINATION
                if(currentRequest.source==elevatorGroup[i].eReq.destination) {
                    found = 1;
                    elevatorGroup[i].eReq = currentRequest;
                    currentRequest.requiredMoves = elevatorGroup[i].process(i);
                    break;
                }
            }
            if(!found) {//if an elevator is not found at the request source then it must move one there for pickup.
                cout<<"NO ELEVATOR FOUND"<<endl<<"GETTING ELEVATOR TO "<<currentRequest.source<<endl;
				currentRequest.requiredMoves += abs((currentRequest.source - currentRequest.destination));
                elevatorGroup[0].eReq.source = currentRequest.source;
                elevatorGroup[0].eReq.destination = elevatorGroup[0].eReq.source;
                elevatorGroup[0].eReq.ppl=0;
                toString(0, elevatorGroup[0]);
            }
        } while(!found);
        cout<<endl<<"AFTER"<<endl;
        showElevatorStatus();
        cout<<"**************************************************"<<endl;
		
    }
};

// File Handler Protocol
class FileHandler {
public:
    virtual void readFromFile()=0;
    virtual void writeToFile(ElevatorRequest)=0;
};

class MyFileHandler:public FileHandler {
private:
    Elevator_Controller controller;
public:
    void readFromFile();
    void writeToFile(ElevatorRequest);
};
void MyFileHandler::writeToFile(ElevatorRequest request) {
    ofstream outfile;
    outfile.open("output.txt", ios::app);
    outfile<<request.source <<" "<<request.destination <<" "<<request.ppl<<" "<<request.requiredMoves<<endl;
    outfile.close();
}
void MyFileHandler::readFromFile() {
    char wait;
    ifstream infile;
    infile.open("input.txt");
    while(!infile.eof()) {
        ElevatorRequest elevator_commands;
        infile>>elevator_commands.source;
        infile>>elevator_commands.destination;
        infile>>elevator_commands.ppl;
		elevator_commands.requiredMoves = 0;
		cout<<"New Elevator Request"<<endl;
		
        elevator_commands.toString();
		controller.process(elevator_commands);
        writeToFile(elevator_commands);
		
    }
}
int main() {
    MyFileHandler fileHandle;
    fileHandle.readFromFile();
    return 0;
}