import java.io.IOException;
import java.util.Collection;
import java.util.LinkedList;

import org.apache.commons.collections.IteratorUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;

import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.join.Parser;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class ProcessUnits8 {
    public static class NodeToChildMapper extends Mapper<Text, Text, Text, Text>{
	        public static final Log log = LogFactory.getLog(NodeToChildMapper.class);
	        public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
			            context.write(key, value);
			        }
	    }

    public static class NodeToChildCombiner extends Reducer<Text, Text, Text, Text> {
	        public void reduce(Text key, Iterable<Text> values, Context context)
	                throws IOException, InterruptedException
	        {
			            LinkedList<Text> list = new LinkedList<Text>();
			            list.add(key);
			            @SuppressWarnings("unchecked")
						            Collection<Text> val_arr = IteratorUtils.toList(values.iterator());
			            list.addAll(val_arr);
			            for (Text child_i : list){
						                for (Text child_j : list){
										                    context.write(child_i, child_j);
										                }
						            }
			        }
	    }

    public static class NodeToChildReducer extends Reducer<Text, Text, Text, Text> {
	        public void reduce(Text key, Iterable<Text> values, Context context)
	                throws IOException, InterruptedException
	        {
			            String out = "";
			            for (Text val : values) {
						                out += " " + val.toString();
						            }
			            Text result = new Text(out);
			            context.write(key, result);
			        }
	    }

    public static void main(String[] args) throws Exception {
	        Configuration conf = new Configuration();
	        Job job = Job.getInstance(conf, "WebApp7No8");
	        job.setInputFormatClass(KeyValueTextInputFormat.class);

	        job.setJarByClass(ProcessUnits8.class);
	        job.setMapperClass(NodeToChildMapper.class);
	        job.setCombinerClass(NodeToChildCombiner.class);
	        job.setReducerClass(NodeToChildReducer.class);

	        job.setOutputKeyClass(Text.class);
	        job.setOutputValueClass(Text.class);

	        FileInputFormat.addInputPath(job, new Path(args[0]));
	        FileOutputFormat.setOutputPath(job, new Path(args[1]));

	        System.out.println("Max split " + FileInputFormat.getMaxSplitSize(job));
	        System.out.println("Min split " + FileInputFormat.getMinSplitSize(job));
	        System.exit(job.waitForCompletion(true) ? 0 : 1);
	    }
}
