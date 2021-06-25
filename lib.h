#define k (2.0*M_PI/360.0)
#define K (1.0/k)
#define Y_min 2021
#define Y_max 2021

class Body{

 public:
  vector<string>::iterator name, type;
  void set(void), print(void);
  
};

void Body::print(void){

  cout << "Name: " << *name << "\n";
  cout << "Type: " << *type << "\n";

}

/*
void Body::set(void){

  bool check;
  string s;



  do{
    cout << "Enter name of body:";
    cin >> s;
    name = find(body_name.begin(), body_name.end(), s);
    
    if(name!=body_name.end()){check = true;}
    else{
      cout << "Entered value is not valid! Try again.\n";
      flush(cout);
      check = false;
    }
  }while(!check);

  print();

  
}
*/



class Time{

public:  int Y, M, D, h, m;
  double s, mjd;
  bool check_Y(void), check_M(void), check_D(void), check_h(void), check_m(void), check_s(void);
  void set(void);
  void print(void);
  void to_mjd(void);
  
};

class Angle{

public:
 
  double value;
  void normalize(void);
  void set(void);
  void print(void);
  
};




class Sight{

public:
  Time t;
  Angle index_error;
  Body body;

};


void Angle::set(void){

  string s;
  int ad;
  double am;
  bool check;
  
  cout << "Enter angle s adadad amam:\n";

  do{
    cout << "Enter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);

  
  do{
    cout << "Enter adadad: ";
    cin >> ad;
    
    if((abs(ad) >= 0) && (abs(ad) < 360)){check = true;}
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

  value = k*(ad + am/60.0);
  if(s=="-"){value*=-1.0;}
  normalize();


}

void Angle::normalize(void){

  value = value - 2.0*M_PI*floor(value/(2.0*M_PI));

}


void Angle::print(void){

  normalize();
  cout << "Value is " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'";

}


bool Time::check_Y(void){

  if((Y >= Y_min) && (Y <= Y_max)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};


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
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_h(void){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_m(void){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_s(void){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
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

  cout << "\nEnter UTC time YYYY MM DD hh-mm-s:\n";

  do{
    cout << "Enter YYYY: ";
    cin >> Y;
  }while(!check_Y());
  
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

void Time:: to_mjd(void)
  
  /*
    Modified Julian Date ( MJD = Julian Date - 2400000.5)
    valid for every date
    Julian Calendar up to 4-OCT-1582,
    Gregorian Calendar from 15-OCT-1582.
  */
{
  
  long int b, c;
   
  mjd = 10000.0 * Y + 100.0 * M + D;
  if (M <= 2)
    {
      M = M + 12;
      Y = Y - 1;
    };
  if (mjd <= 15821004.1)
    {
      b = ((Y+4716)/4) - 1181;
      if (Y < -4716)
        {
	  c = Y + 4717;
	  c = -c;
	  c = c / 4;
	  c = -c;
	  b = c - 1182;
        };
    }
  else b = (Y/400) - (Y/100) + (Y/4);
  //     { b = -2 + floor((Y+4716)/4) - 1179;}
  // else {b = floor(Y/400) - floor(Y/100) + floor(Y/4);};
   
  mjd = 365.0 * Y - 679004.0;
  mjd = mjd + b + int(30.6001 * (M + 1)) + D + (((double)h) + 60.0*((double)m) + 60.0*60.0*((double)s)) / 24.0;
   
  
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


