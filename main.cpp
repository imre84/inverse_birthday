//https://math.stackexchange.com/questions/4964323/inverse-birthday-estimation-2
//for the porposes of the question provided on the link above let's assume that:
//    QRandomGenerator (lott_single) is a true unbiased random number generator
//    unsigned int is able to store arbitrarily big integer numbers
//    lott_single provides an even distribution for arbitrarily big lott_single::max values
//question: what's the limit function of f (generated on the standard output of this application) when magic_parameter approaches positive infinity?

#include <QRandomGenerator>
#include <map>
#include <unordered_set>
#include <iostream>

using namespace std;

class lott_single
{
public:
    typedef unsigned int tRange;
    lott_single(unsigned int max0):max(max0){}
    tRange getRand();
private:
    const unsigned int max;
};

lott_single::tRange lott_single::getRand()
{
    static_assert(QRandomGenerator::min()==0);
    tRange generated;
    do
    {
        generated=QRandomGenerator::global()->generate();
    }
    while((QRandomGenerator::global()->max()%max)!=(max-1)&&((generated/max)==(QRandomGenerator::global()->max()/max)));

    generated%=max;
    //Q_ASSERT(generated<max);
    return generated;
}

template<class T>
unsigned int single_paradox(T &r)
{
    typename T::tRange c=r.getRand();
    unordered_set<typename T::tRange> s;
    while(s.find(c)==s.end())
    {
        s.insert(c);
        c=r.getRand();
    }
    return s.size()+1;
}

int main(int argc, char *argv[])
{
    constexpr unsigned int magic_parameter=1000;
    constexpr unsigned int mindays=10;
    constexpr unsigned int maxdays=magic_parameter;
    constexpr unsigned int maxparadoxes=magic_parameter;
    map<unsigned int,map<unsigned int,unsigned int>> z;
    //z[collision][days]=number of occurrence;
    for(unsigned int days=mindays;days<maxdays;++days)
    {
        //if(99==days%100) cout<<"//days="<<days<<endl;
        lott_single rng(days);
        for(unsigned int i=0;i<maxparadoxes;++i)
        {
            unsigned int collision=single_paradox(rng);
            ++z[collision][days];
        }
    }

#if 0
    //raw data output, doesn't matter
    for(const auto &a:z)
    {
        for(const auto &b:a.second)
        {
            cout<<"collision count="<<a.first<<" when days="<<b.first<<" occurs "<<b.second<<" times."<<endl;
            //cout<<"z["<<a.first<<"]["<<b.first<<"]="<<b.second<<endl;
        }
    }
#endif

    //what we're really after is given a collision what the value of days should be so that z[collision][days]=max of{z[collision][...]}
    for(const auto &a:z)
    {
        unsigned int maxcount=0;
        unsigned int maxdays=0;
        for(const auto &b:a.second)
        {
            if(b.second>maxcount)
            {
                maxcount=b.second;
                maxdays=b.first;
            }
        }
        //cout<<"collision count="<<a.first<<" when days="<<maxdays<<" occurs "<<maxcount<<" times."<<endl;
        cout<<"f("<<a.first<<")="<<maxdays<<"       //maxcount="<<maxcount<<endl;
    }
    return 0;
}
