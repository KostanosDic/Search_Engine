class LowerCaseMaker
{
 public:

    void operator() (char& elem) const
    {
        elem = tolower(elem);
    }
};

