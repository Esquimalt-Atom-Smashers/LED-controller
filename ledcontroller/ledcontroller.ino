typedef struct
{
    int port;
    double value;
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


void setup() 
{
  red = (ledchannel *) malloc(sizeof(ledchannel));
  blue = (ledchannel *) malloc(sizeof(ledchannel));
  green = (ledchannel *) malloc(sizeof(ledchannel));

  initChannel(red, 0.2, 1, 1000, 1, 1, 3);
  initChannel(green, 0, 0, 0, 0, 0, 5); //target color no blue
  initChannel(blue, 0.2, 1, 1000, 1, 1, 6);
  lastTime = millis();
}


void initChannel(ledchannel * channel, double ledmin, double ledmax, double fadetime, double leddirection, double value, int port)
{
  channel->port = port;
  channel->value = value;
  channel->fadetime = fadetime;
  channel->ledmin = ledmin;
  channel->ledmax = ledmax;
  channel->leddirection = leddirection;
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

  analogWrite(channel->port, (int)(channel->value * 255));
}


void loop() 
{
  double now = millis();
  double deltat = now - lastTime;
  lastTime = now;
  updateChannel(deltat, red);
  updateChannel(deltat, green);
  updateChannel(deltat, blue);
}
