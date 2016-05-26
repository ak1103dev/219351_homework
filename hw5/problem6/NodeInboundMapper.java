package th.in.whs.ku;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class NodeInboundMapper extends Mapper<Text, Text, Text, IntWritable> {

    private final static IntWritable one = new IntWritable(1);

    public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        context.write(value, one);
    }
}