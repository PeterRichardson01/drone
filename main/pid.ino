
PID::PID(double DT, double MAX, double MIN, double KP, double KD, double KI)
{
  dt = DT;
  maxi = MAX;
  mini = MIN;
  kp = KP;
  kd = KD;
  ki = KI;
  prev = 0;
  integ = 0;
}

double PID::calculate(double set, double curr)
{
  double err = set - curr;

  double Pout = kp*err;

  integ += err*dt;
  double Iout = ki*integ;

  double deriv = (curr-prev)/dt;
  double Dout = kd*deriv;

  double Out = Pout + Iout + Dout;

  if(Out > maxi)
  {
    Out = maxi;
  }
  else if(Out < mini)
  {
    Out = mini;
  }

  prev = curr;

  return Out;
}
