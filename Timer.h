#pragma once

class Timer {	
  private:
  	int startTicks; //Die Uhrzeit wann der Timer gestartet wurde
  	int pausedTicks; //Uhrzeit bei der der Timer pausiert wurde
  
  	bool paused; //Timerstatus
  	bool started;
  
  public:
  	Timer(); //Initialisierung
  	void start();	//Timeractions
  	void pause();
  	void stop();
  	void unpause();
  
  	int getTicks(); //Uhrzeit zurückgeben
  	bool is_started(); //Status zurückgeben
  	bool is_paused();
};