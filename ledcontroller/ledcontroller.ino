typedef struct
{
    int port;
    int multiplierPot;
    
    double value;
    double multiplier;
    double ledmin;
    double ledmax;
    double ledincrease;
    double leddirection;
    double fadetime;
} ledchannel;

ledchannel *red;
ledchannel *blue;
ledchannel *green;


double lastTime;
int fadeSyncSwitch;


void setup() 
{
  red = (ledchannel *) malloc(sizeof(ledchannel));
  blue = (ledchannel *) malloc(sizeof(ledchannel));
  green = (ledchannel *) malloc(sizeof(ledchannel));

  initChannel(red, 0 , 1, 1500, 1, 1, 3,1);
  initChannel(green, 0, 0, 200, 1, 1, 5,0); //target color no blue
  initChannel(blue, 0, 1, 1500, 1, 1, 6,0);
  lastTime = millis();
  Serial.begin(9600);
}


float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void initChannel(ledchannel * channel, double ledmin, double ledmax, double fadetime, double leddirection, double value, int port, double multiplier)
{
  channel->port = port;
  channel->value = value;
  channel->fadetime = fadetime;
  channel->ledmin = ledmin;
  channel->ledmax = ledmax;
  channel->leddirection = leddirection;
  channel->multiplier = multiplier;
  channel->ledincrease = (ledmax - ledmin)/fadetime;
  pinMode(port, OUTPUT);
}

void updateChannel(double deltat, ledchannel *channel)
{
  double increase = deltat * channel->ledincrease * channel->leddirection;
  channel->value += increase;

  if (channel->value > channel->ledmax)
  {
    channel->value = channel->ledmax;
    channel->leddirection *= -1.0;
  }
  else if (channel->value < channel-> ledmin)
  {
    channel->value = channel->ledmin;
    channel->leddirection *= -1.0;
  }

  analogWrite(channel->port, (int)(channel->value * 255 * channel->multiplier));

}


void loop() 
{
  double now = millis();
  double deltat = now - lastTime;
  lastTime = now;
  updateChannel(deltat, red);
  updateChannel(deltat, green);
  updateChannel(deltat, blue);

  double r = analogRead(0);
  red->multiplier = mapfloat(r,0,1023,0,1);
 
}
