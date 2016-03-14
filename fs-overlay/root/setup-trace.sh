#!/bin/sh

#echo function_graph > current_trace
#echo 10 > max_graph_depth
#echo  > set_graph_function


#trace-cmd record -p function --func-stack


trace-cmd record -p function_graph -l rtw_enqueue_cmd


