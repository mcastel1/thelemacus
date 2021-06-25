#define k (2.0*M_PI/360.0)
#define K (1.0/K)

class Time{

public:  int Y, M, D, h, m;
  double s;
  bool check_M(void), check_D(void), check_h(void), check_m(void), check_s(void);
  void set(void);
  void print(void);
  
};

class Angle{

public:
  double value;
  void set(void);
  
};

class Sight{

public:
  Time t;
  Angle index_error;

};


 

void Angle::set(void){

  unsigned int ad;
  double am;
  bool check;
  
  cout << "\nEnter angle adadad amam:";

  do{
    cout << "\nEnter adadad: ";
    cin >> ad;
    
     if((ad >= 0) && (ad < 360)){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);
  
  do{
    cout << "Enter amam: ";
    cin >> am;

    if((am >= 0.0) && (am < 60.0)){check = true;}
    else{
      cout << "Entered value is not valid! Try again.\n";
      flush(cout);
      check = false;
    }
  }while(!check);

  value = k*(((double)ad) + am/60.0);

  cout << "Entered value is " << ad << "° " << am;

}



bool Time::check_M(void){

  if((M >= 1) && (M <= 12)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_D(void){

  if((D >= 1) && (D <= 31)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_h(void){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_m(void){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_s(void){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

void Time::print(void){

  cout << "Time is " << Y << " ";
  if(M<10){cout << 0;}
  cout << M << " ";
  if(D<10){cout << 0;}
  cout << D << " ";
  if(h<10){cout << 0;}
  cout << h << "-";
  if(m<10){cout << 0;}
  cout << m << "-";
  if(m<10.0){cout << 0;}
  cout << s << " UTC.\n";
  flush(cout);

};




void Time::set (void) {

  cout << "\nEnter UTC time YYYY MM DD hh-mm-s:";

  cout << "\nEnter YYYY: ";
  cin >> Y;

  do{
    cout << "Enter MM: ";
    cin >> M;
  }while(!check_M());

  do{
    cout << "Enter DD: ";
    cin >> D;
  }while(!(check_D()));

  do{
    cout << "Enter hh: ";
    cin >> h;
  }while(!(check_h()));

  do{
    cout << "Enter mm: ";
    cin >> m;
  }while(!(check_m()));

  do{
    cout << "Enter ss: ";
    cin >> s;
  }while(!(check_s()));

}

/*
  void Sight::set_index_error (void) {

  cout << "\nEnter index error +- ddd\260 mm':";

  cout << "\nEnter +-: ";
  cin >> t.index_error;

  do{
  cout << "\nEnter MM: ";
  cin >> t.M;
  }while(!t.check_M());

  do{
  cout << "\nEnter DD: ";
  cin >> t.D;
  }while(!(t.check_D()));

  do{
  cout << "\nEnter hh: ";
  cin >> t.h;
  }while(!(t.check_h()));

  do{
  cout << "\nEnter mm: ";
  cin >> t.m;
  }while(!(t.check_m()));

  do{
  cout << "\nEnter ss: ";
  cin >> t.s;
  }while(!(t.check_s()));

  }

*/
