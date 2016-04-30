func repeatString_(int i, string str)
{
    return str;
}

func repeatString(int i, string str)
{
    string ret = "blubber";
    { string ret = "bla";}
    ret.append(str);
    return ret;
}
func complexRepeatString(int i, string str)
{
    string ret;
    for(int t = 0; t < i; t++)
    {
        ret.append(str);
    }
    return ret;
}