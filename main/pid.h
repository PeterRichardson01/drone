class PID
{
  public:
    PID(double DT, double MAX, double MIN, double KP, double KI, double KD);
    double calculate(double set, double curr);
  private:
    double dt;
    double maxi;
    double mini;
    double kp;
    double kd;
    double ki;
    double prev;
    double integ;
};
