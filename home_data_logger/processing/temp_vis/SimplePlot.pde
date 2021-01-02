
import java.util.*;

class SimplePlot {

  LinkedList<Double> data_queue = new LinkedList<Double>();
  int buffer_size = -1;

  double value_min = 0;
  double value_max = 0;
  double value_range = 0;
  double gain = 1;


  SimplePlot(int buffer_size_in, double value_min_in, double value_max_in) {
    buffer_size = buffer_size_in;
    value_min = value_min_in;
    value_max = value_max_in;
    value_range = value_max_in - value_min_in;
  }

  void setGain(double gain_in)
  {
    gain = gain_in;
  }

  void addData(double data)
  {
    if(data < value_min)
    {
     data = value_min; 
    }
    
    data = gain*((data - value_min)/value_range);
    if (data_queue.size() < buffer_size)
    {
      data_queue.addLast(data);
    } else
    {
      data_queue.removeFirst();
      data_queue.addLast(data);
    }
  }

  void plot()
  {

    int offset_Y = 100;

    PShape path = createShape();
    path.beginShape();
    path.noFill();
    path.stroke(255);
    //System.out.println("data_queue.size() = "+data_queue.size());
    for (int i = 0; i < data_queue.size(); i++) {
      double temp_data = data_queue.get(i);
      //System.out.println(linkedList.get(i));
      path.vertex(i, offset_Y - (int)temp_data);
    }
    // etc;
    path.endShape();

    shape(path);
  }
}
