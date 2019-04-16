#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>
#include<stdio.h>
#include <time.h>
using namespace std;



int cWinCtr = 0;


struct Attack
{
  string name;
  int power;
  string type;
  int statFlag;
  string statDmgType;
  double statDamage;
  int mtype; //0 - physical, 1 - special
};

struct Pokemon
{
  int level;
  Attack atts[4];
  string name;
  double attack;
  double defense;
  double speed;
  double spAttack;
  double spDefense;
  double accuracy;
  string type;
  int hp;
};

struct moveEffect
{

  int statFlag;
  int damage;
  string stat;
  double statDamage;

};

Pokemon charizard, venusaur;
Attack ember, tackle, vinewhip, leer, growl, defensecurl;
map<string,Attack> moveID;

void createAttacks()
{
  vinewhip.name = "vinewhip";
  vinewhip.power = 45;
  vinewhip.type = "grass";
  vinewhip.statFlag = 0;
  vinewhip.statDamage = 0.0;
  vinewhip.mtype = 0;

ember.name = "ember";
ember.power = 40;
ember.type = "fire";
ember.statFlag = 0;
ember.statDamage = 0.0;
ember.mtype = 1;

tackle.name = "tackle";
tackle.power = 20;
tackle.type = "normal";
tackle.statFlag = 0;
tackle.statDamage = 0.0;
tackle.mtype = 0;

leer.name = "leer";
leer.power = 0;
leer.type = "normal";
leer.statFlag = 1;
leer.statDamage = 0.66;
leer.mtype = 0;
leer.statDmgType = "defense";

growl.name = "growl";
growl.power = 0;
growl.type = "normal";
growl.statFlag = 1;
growl.statDamage = 0.66;
growl.mtype = 0;
growl.statDmgType = "attack";

defensecurl.name = "defensecurl";
defensecurl.power = 0;
defensecurl.type = "normal";
defensecurl.statFlag = 2;
defensecurl.statDamage = 1.50;
defensecurl.mtype = 0;
defensecurl.statDmgType = "defense";

moveID["ember"] = ember;
moveID["tackle"] = tackle;
moveID["vinewhip"] = vinewhip;
moveID["leer"] = leer;
moveID["growl"] = growl;
moveID["defensecurl"] = defensecurl;
}

void setPokemonStats()
{

  createAttacks();


  charizard.attack = 50;
  charizard.defense = 50;
  charizard.speed = 150;
  charizard.spAttack = 50;
  charizard.spDefense = 50;

  charizard.name = "charizard";
  charizard.type = "fire";
  charizard.level = 50;
  charizard.hp = 45;

  charizard.atts[0] = tackle;
  charizard.atts[1] = growl;
  charizard.atts[2] = defensecurl;
  charizard.atts[3] = leer;

  venusaur.attack = 50;
  venusaur.defense = 50;
  venusaur.speed = 50;
  venusaur.spAttack = 50;
  venusaur.spDefense = 50;
  venusaur.speed = 150;
  venusaur.name = "venusaur";
  venusaur.type = "grass";
  venusaur.level = 50;
  venusaur.hp = 45;

  venusaur.atts[0] = defensecurl;
  venusaur.atts[1] = tackle;
  venusaur.atts[2] = leer;
  venusaur.atts[3] = growl;



}

double typeEffCheck(string a, string b)
{

  if(a=="fire" && b=="grass")
    return 2.0;
  else if(a=="grass" && b=="fire")
    return 0.5;
  return 1.0;
}

int dmgCalculation(Pokemon a, Pokemon b, Attack c)
{

  if(c.statFlag==1 || c.statFlag == 2)
    return 0.0;
  int lvlTerm = (2*a.level)/5;
  lvlTerm += 2;

  double nmr = lvlTerm * c.power * 1.0;

  if(c.mtype == 0)
    nmr*= (a.attack / (b.defense));
  else
    nmr*= (a.spAttack / ( b.spDefense));

  nmr/=50.0;
  nmr+=2;
  double modifier = typeEffCheck(c.type,b.type);

  return (int)(nmr * modifier);
}

int log2(int n)
{
  return (n==1)? 0 : 1 + log2(n/2);
}

moveEffect move(Pokemon a, Pokemon b, Attack c)
{
  moveEffect e;
  if(c.statFlag == 0)
  {
    e.statFlag = 0;
    e.damage = dmgCalculation(a,b,c);
    e.stat = "";
    e.statDamage = 0.0;
  }

  else
  {
    e.damage = 0;
    e.statFlag = c.statFlag;
    e.stat = c.statDmgType;
    e.statDamage = c.statDamage;
  }
  return e;
}


vector<pair<int,string> > generateGameTree1(Pokemon a, Pokemon b)
{ vector<pair<int,string> > leafs;

  for(int i=0; i<4; i++)
  {

    for(int j=0;j<4;j++)
    {
      for(int k=0;k<4;k++)
      {
        //use attack i, then attack j, then attack k
        Pokemon aa = a, bb = b;
        string aSeq = a.atts[i].name+" - "+b.atts[j].name+" - "+a.atts[k].name;
  //      cout<<a.atts[i].name<<" - "<<b.atts[j].name<<" - "<<a.atts[k].name<<endl;
    //    cout<<i<<" "<<j<<" "<<k<<endl;
        int totalDamage = 0;
    //    cout<<aa.defense<<" "<<bb.defense<<endl;
        moveEffect m1 = move(aa,bb,aa.atts[i]);
        if(m1.statFlag == 0)
        {
          totalDamage += m1.damage;
        }
        else
        if(m1.statFlag == 2)
        {
          if(m1.stat == "attack")
            aa.attack *= m1.statDamage;
          if(m1.stat == "defense")
            aa.defense *= m1.statDamage;
          if(m1.stat == "speed")
            aa.speed *= m1.statDamage;
          if(m1.stat == "spAttack")
              aa.spAttack *= m1.statDamage;
          if(m1.stat == "spDefense")
              aa.spDefense *= m1.statDamage;
        }
        else
        {
          if(m1.stat == "attack")
            bb.attack *= m1.statDamage;
          if(m1.stat == "defense")
            bb.defense *= m1.statDamage;
          if(m1.stat == "speed")
            bb.speed *= m1.statDamage;
          if(m1.stat == "spAttack")
              bb.spAttack *= m1.statDamage;
          if(m1.stat == "spDefense")
              bb.spDefense *= m1.statDamage;

        }
    //    cout<<aa.defense<<" "<<bb.defense<<endl;
        moveEffect m2 = move(bb,aa,bb.atts[j]);
        if(m2.statFlag == 0)
        {
          totalDamage -= m2.damage;
        }
        else if(m2.statFlag == 2)
        {
          if(m2.stat == "attack")
            bb.attack *= m2.statDamage;
          if(m2.stat == "defense")
            bb.defense *= m2.statDamage;
          if(m2.stat == "speed")
            bb.speed *= m2.statDamage;
          if(m2.stat == "spAttack")
              bb.spAttack *= m2.statDamage;
          if(m2.stat == "spDefense")
              bb.spDefense *= m2.statDamage;
        }
        else
        {
          if(m2.stat == "attack")
            aa.attack *= m2.statDamage;
          if(m2.stat == "defense")
            aa.defense *= m2.statDamage;
          if(m2.stat == "speed")
            aa.speed *= m2.statDamage;
          if(m2.stat == "spAttack")
              aa.spAttack *= m2.statDamage;
          if(m2.stat == "spDefense")
              aa.spDefense *= m2.statDamage;

        }
  //      cout<<aa.defense<<" "<<bb.defense<<endl;
        moveEffect m3 = move(aa,bb,aa.atts[k]);
        if(m3.statFlag == 0)
        {
          totalDamage += m3.damage;
        }
        else if(m3.statFlag == 2)
        {
          if(m3.stat == "attack")
            aa.attack *= m3.statDamage;
          if(m3.stat == "defense")
            aa.defense *= m3.statDamage;
          if(m3.stat == "speed")
            aa.speed *= m3.statDamage;
          if(m3.stat == "spAttack")
              aa.spAttack *= m3.statDamage;
          if(m3.stat == "spDefense")
              aa.spDefense *= m3.statDamage;
        }
        else
        {
          if(m3.stat == "attack")
            bb.attack *= m3.statDamage;
          if(m3.stat == "defense")
            bb.defense *= m3.statDamage;
          if(m3.stat == "speed")
            bb.speed *= m3.statDamage;
          if(m3.stat == "spAttack")
              bb.spAttack *= m3.statDamage;
          if(m3.stat == "spDefense")
              bb.spDefense *= m3.statDamage;

        }
  //      cout<<aa.defense<<" "<<bb.defense<<endl;
        leafs.push_back(make_pair(totalDamage, aSeq));
  //      cout<<"Damage caused "<<totalDamage<<endl;
      }
    }
  }
  return leafs;
}

vector<moveEffect> generateME(Pokemon a, Pokemon b)
{
  vector<moveEffect> mes;
  for(int i=0;i<4;i++)
  {
    mes.push_back(move(a,b,a.atts[i]));
  }
  return mes;
}
/*
int minimax(int depth, int nodeIndex, bool isMax,
            vector<int> scores, int h)
{

    if (depth == h)
        return scores[nodeIndex];

    if (isMax)
       return max( minimax(depth+1, nodeIndex*4, false, scores, h),
            max(minimax(depth+1, nodeIndex*4 + 1, false, scores, h),
            max( minimax(depth+1, nodeIndex*4 + 2, false, scores, h) , minimax(depth+1, nodeIndex*4 + 3, false, scores, h) )) );
    else
        return min( minimax(depth+1, nodeIndex*4, true, scores, h),
            min(minimax(depth+1, nodeIndex*4 + 1, true, scores, h),
            min( minimax(depth+1, nodeIndex*4 + 2, true, scores, h),  minimax(depth+1, nodeIndex*4 + 3, true, scores, h) )) );
}
*/
pair<int,string> minimax1(int depth, int nodeIndex, bool isMax,
            vector< pair<int,string> > scores, int h)
{

    if (depth == h)
        return scores[nodeIndex];

    if (isMax)
    {

        vector<pair<int,string> >a;
        a.push_back(minimax1(depth+1, nodeIndex*4, false, scores, h));
        a.push_back(minimax1(depth+1, nodeIndex*4 + 1, false, scores, h));
        a.push_back(minimax1(depth+1, nodeIndex*4 + 2, false, scores, h));
        a.push_back( minimax1(depth+1, nodeIndex*4 + 3, false, scores, h));
        sort(a.begin(),a.end());
        return a[3];
    }

    else {

      vector<pair<int,string> >a;
      a.push_back(minimax1(depth+1, nodeIndex*4, true, scores, h));
      a.push_back(minimax1(depth+1, nodeIndex*4 + 1, true, scores, h));
      a.push_back(minimax1(depth+1, nodeIndex*4 + 2, true, scores, h));
      a.push_back( minimax1(depth+1, nodeIndex*4 + 3, true, scores, h));
      sort(a.begin(),a.end());
      return a[0];
    }

}

void charizardVsVenusaur()
{ int flag = 0;
  while(charizard.hp > 0 && venusaur.hp > 0)
  {
    int i = 0;
    if(charizard.speed > venusaur.speed || (charizard.speed == venusaur.speed && rand()%2 == 0))
    {

      vector< pair<int,string > > leafs1 = generateGameTree1(charizard,venusaur);
      pair<int, string> bestMove = minimax1(0,0,true,leafs1,3);
      string seq = bestMove.second;

      string mov = "";

      while(seq[i]!= ' ') {
        mov+=seq[i];
        i++;
      }
    //  cout<<" i is at "<<i<<" "<<seq[i]<<endl;
      moveEffect e1 = move(charizard, venusaur,moveID[mov]);
  //    cout<<"charizard used "<<mov<<endl;
      if(e1.statFlag == 0)
      {
        venusaur.hp -= e1.damage;
      }
      else
      {
        if(e1.stat == "attack")
          venusaur.attack *= e1.statDamage;
        if(e1.stat == "defense")
          venusaur.defense *= e1.statDamage;
        if(e1.stat == "speed")
          venusaur.speed *= e1.statDamage;
        if(e1.stat == "spAttack")
            venusaur.spAttack *= e1.statDamage;
        if(e1.stat == "spDefense")
            venusaur.spDefense *= e1.statDamage;
      }


    //  cout<<"venusaur has "<<venusaur.hp<<" HP left\n";
      if(venusaur.hp <= 0) {
        cout<<"Venusaur fainted\n\n";
        break;
      }


        int batt = (rand()%4) + 1;
        //cout<<batt<<endl;

        e1 = move(venusaur, charizard,venusaur.atts[batt]);
    //   cout<<"venusaur used "<<venusaur.atts[batt].name<<endl;
       if(e1.statFlag == 0)
       {
         charizard.hp -= e1.damage;
       }
       else
       {
         if(e1.stat == "attack")
           charizard.attack *= e1.statDamage;
         if(e1.stat == "defense")
           charizard.defense *= e1.statDamage;
         if(e1.stat == "speed")
           charizard.speed *= e1.statDamage;
         if(e1.stat == "spAttack")
             charizard.spAttack *= e1.statDamage;
         if(e1.stat == "spDefense")
             charizard.spDefense *= e1.statDamage;
       }

      // cout<<"charizard has "<<charizard.hp<<" HP left\n";
       if(charizard.hp <= 0) {
         cout<<"charizard fainted\n\n";
         flag = 1;
         break;
       }
    }
    else
    {
      int batt = (rand()%4) + 1;
      //cout<<batt<<endl;

      moveEffect e1 = move(venusaur, charizard,venusaur.atts[batt]);
  //   cout<<"venusaur used "<<venusaur.atts[batt].name<<endl;
     if(e1.statFlag == 0)
     {
       charizard.hp -= e1.damage;
     }
     else
     {
       if(e1.stat == "attack")
         charizard.attack *= e1.statDamage;
       if(e1.stat == "defense")
         charizard.defense *= e1.statDamage;
       if(e1.stat == "speed")
         charizard.speed *= e1.statDamage;
       if(e1.stat == "spAttack")
           charizard.spAttack *= e1.statDamage;
       if(e1.stat == "spDefense")
           charizard.spDefense *= e1.statDamage;
     }

    // cout<<"charizard has "<<charizard.hp<<" HP left\n";
     if(charizard.hp <= 0) {
       cout<<"charizard fainted\n\n";
       flag = 1;
       break;
     }

     int i = 0;
     if(charizard.speed >= venusaur.speed)
     {

       vector< pair<int,string > > leafs1 = generateGameTree1(charizard,venusaur);
       pair<int, string> bestMove = minimax1(0,0,true,leafs1,3);
       string seq = bestMove.second;

       string mov = "";

       while(seq[i]!= ' ') {
         mov+=seq[i];
         i++;
       }
     //  cout<<" i is at "<<i<<" "<<seq[i]<<endl;
       e1 = move(charizard, venusaur,moveID[mov]);
   //    cout<<"charizard used "<<mov<<endl;
       if(e1.statFlag == 0)
       {
         venusaur.hp -= e1.damage;
       }
       else
       {
         if(e1.stat == "attack")
           venusaur.attack *= e1.statDamage;
         if(e1.stat == "defense")
           venusaur.defense *= e1.statDamage;
         if(e1.stat == "speed")
           venusaur.speed *= e1.statDamage;
         if(e1.stat == "spAttack")
             venusaur.spAttack *= e1.statDamage;
         if(e1.stat == "spDefense")
             venusaur.spDefense *= e1.statDamage;
       }

       if(venusaur.hp <= 0) {
         cout<<"Venusaur fainted\n\n";
         break;
       }

     }
    }

  }
  if(flag == 0)
    cWinCtr++;
}


map< pair<pair<int,int>,int >,double> qTable;

void initQTable()
{
  for(int i=0;i<=50;i++)
    for(int j=0;j<=50;j++) {

      qTable[make_pair(make_pair(i,j),0)] = 0;
      qTable[make_pair(make_pair(i,j),1)] = 0;
      qTable[make_pair(make_pair(i,j),2)] = 0;
      qTable[make_pair(make_pair(i,j),3)] = 0;

      if(j==0)
      {
        qTable[make_pair(make_pair(i,j),0)] = 100;
        qTable[make_pair(make_pair(i,j),1)] = 100;
        qTable[make_pair(make_pair(i,j),2)] = 100;
        qTable[make_pair(make_pair(i,j),3)] = 100;
      }
      if(i==0)
      {
        qTable[make_pair(make_pair(i,j),0)] = -100;
        qTable[make_pair(make_pair(i,j),1)] = -100;
        qTable[make_pair(make_pair(i,j),2)] = -100;
        qTable[make_pair(make_pair(i,j),3)] = -100;
      }


    }
}

double alpha = 0.1;
double epsilon = 0.2;
double gamma = 0.9;

void initQLearn()
{

}

void displayQTable()
{
  for(int i=0;i<50;i++)
  {
    for(int j=0;j<50;j++)
    {

        cout<<"("<<qTable[make_pair(make_pair(i,j),0)] << "," << qTable[make_pair(make_pair(i,j),1)]
         << "," << qTable[make_pair(make_pair(i,j),2)] << "," << qTable[make_pair(make_pair(i,j),3)] << ")  ";

    }
    cout<<endl;
  }
}

void episodeRun(Pokemon a, Pokemon b, pair<int,int> curState)
{
  int randNumber = (rand()%10) + 1;
  int bestMove = 0;
  for(int i=1;i<4;i++)
  {
    if(qTable[make_pair(curState,i)] > qTable[make_pair(curState,bestMove)])
    {
      bestMove = i;
    }
  }
  int moveToMake;
  if(randNumber >= 8)
  {
    moveToMake = (rand()%4);
    cout<<"Picking move "<<moveToMake <<" Exploration wins\n\n";
  }
  else {
    moveToMake = bestMove;
    cout<<"Picking move "<<moveToMake<<" - "<<a.atts[moveToMake].name<<" Exploitation wins\n\n";
  }

  //computing reward for using moveToMake
  moveEffect m = move(a,b,a.atts[moveToMake]);
  cout<<"damage = " << m.damage<<"  Stat affected = "+m.stat<<endl;

  int reward = 0;
  if(m.damage > 0)
    reward += m.damage;

    //ADD REWARD COMPUTATION FOR STAT DAMAGE HERE

  double update = qTable[make_pair(curState,moveToMake)] ;
  //finding max Q(s', a')
  cout<<"Reward = "<<reward<<endl;
  pair<int,int> futureState = make_pair(a.hp,max(b.hp - m.damage,0));
  cout<<"Future state = "<<futureState.first<<" HP, "<<futureState.second<<" HP\n\n";
  double bestFutureState =  qTable[make_pair(futureState,0)];
  for(int i=1;i<4;i++)
  {
    if(bestFutureState < qTable[make_pair(futureState,i)])
      bestFutureState = qTable[make_pair(futureState,i)];
  }

  update += alpha * ( reward + gamma*bestFutureState - qTable[make_pair(curState,moveToMake)]);

  cout<<"We update the current state-action pair's value to "<<update<<endl;
  qTable[make_pair(curState,moveToMake)] = update;

}

void QLearning()
{
  pair<int,int> p = make_pair(charizard.hp,venusaur.hp);
  episodeRun(charizard, venusaur, p);

}

int main()
{
  srand(time(0));
  setPokemonStats();
  QLearning();
  displayQTable();
/*
  Pokemon c = charizard, v = venusaur;
  for(int i=0;i<5000;i++) {
    charizardVsVenusaur();
    charizard = c;
    venusaur = v;
  }
  cout<<cWinCtr<<endl;
*/
  return 0;
}
