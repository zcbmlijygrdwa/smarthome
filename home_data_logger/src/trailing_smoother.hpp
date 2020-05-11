
class TrailingSmoother
{
    public:

        double weight = 0.1;
        double trailing_data = 0;
        bool is_init = false;

        TrailingSmoother(double weight_in)
        {
            weight = weight_in;
        }

        double add(double data_in)
        {
            if(is_init)
            {
                trailing_data =  trailing_data + weight*(data_in - trailing_data);
            }
            else
            {
                trailing_data = data_in;
                is_init  = true;
            }
            return trailing_data;
        }

};
