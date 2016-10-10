# CS5331_Visualization_Mano_Project 1

## Unemployment Visualization Web Application
A Web application to anaylize the the US Unemployment rates over the date ranges from 1978 to the current month.

## Sources
The application uses the seasonally not adusted state wide unemployment rates from the [BLS Website](http://www.bls.gov/). For map, it  uses the US Shape JSON file from the [Census IRE Website](http://census.ire.org/)

## How it works?

Application mainly divided into two menus
- States _vs_ Time Range
- Different States _vs_ Different Years

### States _vs_ Time Range

By default, the application shows the National Unemployment rate through line graph that distributed from the 1976 to 2011. It has the check box option to select/deselect the National Unemployment from the graph.

Using the Map, we can select/deselect the particular state to plot its respective employment rate to the Graph Area.
The selected states in the map and graph can be distinguised with the automatically associated colors.

Required data range can be set using the provided slider. The line graphs are automatically replotted as per the new  selected data range.

_**Zoom - Graph Area**_

Line graphs can be easily compared using the zoomable option. Select the area on the graph to zoom the content. Double click on the graph to reset the content back to previous state.

_**Highlight - Unemployment rate changes**_

The months that experianced specific change rates can be highlighted using the Sudden change rates checkbox. First select the menu and then enter the rate that we want to observe. On selecting the state from the map, the line grpah will be plotted using the black and red markings(circles). 
Black and Red circles represent the succussive months when that sudden change rate happened.

### Different States _vs_ Different Years

With this option, we can compare the different states and different years.
Select the States vs Years menu. On selecting the states from the menu, it will create enries with State name and an input box for the Year. Enter the years in the provided text box. Then it will plot the graph for the selected state and the year.

We can remove the states from the graph area by clicking on the X icon infront of the state name.

## Interesting Observations

Rates during Recessions

As per National Bureau of Economic Research, the major recession period is during 2007-09. The number of long-term unemployed—those jobless 27 weeks or longer—has remained high years after the recession ended in June 2009.
We can clearly observe those trends here in below graphs




### Credits
[Tommy Dang](https://github.com/TuanNhonDang)he number of long-term unemployed—those jobless 27 weeks or longer—has remained high years after the recession ended in June 2009

[Mike Bostock](http://bl.ocks.org/mbostock) (D3 Examples)

## Future Work

There is lot of scope to improve the application by adding the features that help for the analysis. Some of those features are
- Clustering - We can make use of javascript library [kmeans-js](https://www.npmjs.com/package/kmeans-js) for clustering.
- UnEmployment Predicton - 
- Integrating with other parameters like inflation rates, industry wise trends








    