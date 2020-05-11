
#include <queue>
#include <thread>
#include <mutex>

class SensorReader
{
    private:
    public:
        static std::mutex mutex_lock;
        struct DataFormat
        {
            bool is_data_good = false;
            long long unix_time = -1;
            double light = -1;
            double soundPressure = -1;
            double motionX = -1;
            double motionY = -1;
            double motionZ = -1;
            double temperature = -1;

            DataFormat(std::string data_str, long long unix_time_in)
            {
                //printv(unix_time_in);
                size_t pos = 0;
                std::string token;
                std::string delimiter = ",";
                std::vector<double> data_vec;
                while ((pos = data_str.find(delimiter)) != std::string::npos)
                {
                    token = data_str.substr(0, pos);
                    //std::cout << token << std::endl;
                    data_vec.push_back(std::stod(token));
                    data_str.erase(0, pos + delimiter.length());
                }
                data_vec.push_back(std::stod(data_str));
                //printv(data_vec.size());
                if(data_vec.size() == 6)
                {
                    unix_time = unix_time_in;
                    is_data_good = true;
                    light         = data_vec[0];
                    soundPressure = data_vec[1];
                    motionX       = data_vec[2];
                    motionY       = data_vec[3];
                    motionZ       = data_vec[4];
                    temperature   = data_vec[5];
                }
            }

            DataFormat()
            {
                unix_time = -1;
            }

            std::string str()
            {
                std::stringstream ss;
                if(is_data_good)
                {
                    ss<<"unix_time: "<<unix_time<<", light: "<<light<<", soundPressure: "<<soundPressure<<", acc: {"<<motionX<<","<<motionY<<","<<motionZ<<"}, temperature: "<<temperature;
                }
                else
                {
                    ss<<"BadData...";
                }
                return ss.str();
            }

            std::string strLite()
            {
                std::stringstream ss;
                ss<<unix_time<<","<<light<<","<<soundPressure<<","<<motionX<<","<<motionY<<","<<motionZ<<","<<temperature;
                return ss.str();
            }
        };

        std::queue<DataFormat>* queue;

        serial::Serial* my_serial;

        SensorReader(std::string port, unsigned long long baud)
        {
            // port, baudrate, timeout in milliseconds
            my_serial = new serial::Serial(port, baud, serial::Timeout::simpleTimeout(10));
            queue = new std::queue<DataFormat>();

            std::cout << "Is the serial port open?";
            printv(my_serial);
            if(my_serial->isOpen())
                std::cout << " Yes." << std::endl;
            else
                std::cout << " No." << std::endl;

        }

        ~SensorReader()
        {
            std::cout<<"Destruct SensorReader"<<std::endl;
            delete(my_serial);
            delete(queue);
        }

        static void my_sleep(unsigned long long milliseconds) {
#ifdef _WIN32
            Sleep(milliseconds); // 100 ms
#else
            usleep(milliseconds*1000); // 100 ms
#endif
        }

        void enumerate_ports()
        {
            std::vector<serial::PortInfo> devices_found = serial::list_ports();

            std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

            while( iter != devices_found.end() )
            {
                serial::PortInfo device = *iter++;

                printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
                        device.hardware_id.c_str() );
            }
        }

        static void read(serial::Serial* my_serial, std::queue<DataFormat>* queue)
        {
            printv(1);
            printv(my_serial);
            printv(2);
            std::string buffer;
            std::string temp_data_str;
            int count = 0;

            while(true)
            {
                std::string result = my_serial->read(1000);
                buffer = buffer + result;

                //std::cout << "Iteration: " << count << ", Bytes read: ";
                //std::cout << result.length() << ", String read: " << result << std::endl;
                int first_occur = buffer.find('\n');
                if(first_occur > 0)
                {
                    // if find \n, take the data from buffer and parse it
                    temp_data_str = buffer.substr (0,first_occur);
                    //printv(temp_data_str);
                    long long microsecondsUTC = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    //printv(microsecondsUTC);
                    DataFormat df(temp_data_str,microsecondsUTC);
                    //printv(df.str());
                    if(df.is_data_good)
                    {
                        //printv(df.strLite());
                        mutex_lock.lock(); 
                        queue->push(df);
                        mutex_lock.unlock(); 
                    }
                    buffer = buffer.substr(first_occur+1);
                    //printv(buffer);
                }
                //printv(first_occur);
                count += 1;
                my_sleep(100);
            }
        }

        void startReading()
        {
            printv("before thread");
            std::thread thread_obj(read, my_serial, queue);
            std::cout<<"waiting thread_obj to join.."<<std::endl;
            //thread_obj.join();
            thread_obj.detach();
        }

        DataFormat fetchData()
        {
            DataFormat df;
            mutex_lock.lock();
            if(queue->size()>0)
            {
                df = queue->front();
                queue->pop();
            }
            //printv(queue->size());
            mutex_lock.unlock();
            return df;
        }

};

std::mutex SensorReader::mutex_lock;
