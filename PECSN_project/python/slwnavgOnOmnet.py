import math
from omnetpp.scave import results, chart, utils
import pandas as pd

# get chart properties
props = chart.get_properties()
utils.preconfigure_plot(props)

# collect parameters for query
filter_expression = props["filter"]
start_time = float(props["vector_start_time"] or -math.inf)
end_time = float(props["vector_end_time"] or math.inf)

# query vector data into a data frame
try:
    df = results.get_vectors(filter_expression, include_attrs=True, include_runattrs=True, include_itervars=True,
                             start_time=start_time, end_time=end_time)
except results.ResultQueryError as e:
    raise chart.ChartScriptError("Error while querying results: " + str(e))

if df.empty:
    raise chart.ChartScriptError("The result filter returned no data.")

# apply vector operations
df = utils.perform_vector_ops(df, props["vector_operations"])
print(pd.Series(df['vecvalue'])[0])
# calculate sliding window average
window_size = 10  # adjust the window size as needed
xa = pd.Series(df['vecvalue'][0])
df['vecvalue'][0] = xa.rolling(window=1000).mean()

# plot
utils.plot_vectors(df, props)

utils.postconfigure_plot(props)

utils.export_image_if_needed(props)
utils.export_data_if_needed(df, props)

#---------------------------------------------


#if multiple lines
import math
from omnetpp.scave import results, chart, utils
import pandas as pd

# get chart properties
props = chart.get_properties()
utils.preconfigure_plot(props)

# collect parameters for query
filter_expression = props["filter"]
start_time = float(props["vector_start_time"] or -math.inf)
end_time = float(props["vector_end_time"] or math.inf)

# query vector data into a data frame
try:
    df = results.get_vectors(filter_expression, include_attrs=True, include_runattrs=True, include_itervars=True,
                             start_time=start_time, end_time=end_time)
except results.ResultQueryError as e:
    raise chart.ChartScriptError("Error while querying results: " + str(e))

if df.empty:
    raise chart.ChartScriptError("The result filter returned no data.")

# apply vector operations
df = utils.perform_vector_ops(df, props["vector_operations"])
print(df.describe())
print(pd.Series(df['vecvalue'])[0])
# calculate sliding window average
window_size = 1000  # adjust the window size as needed
for k in range(0, len(df['vecvalue'])):
    print(k)
    xa = pd.Series(df['vecvalue'][k])
    df['vecvalue'][k] = xa.rolling(window=window_size).mean()

# plot
utils.plot_vectors(df, props)

utils.postconfigure_plot(props)

utils.export_image_if_needed(props)
utils.export_data_if_needed(df, props)
