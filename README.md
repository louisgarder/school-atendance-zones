# school-atendance-zones
Program to create desegregated school attendance zones in a region using US census data

This is one of of the first programs that attempts to create desegregated school attendance zones using race/ethnicity demographic data from the US census database. A school attendance zone
is an outlined region where students will attend public school. A common problem with current school attendance zones is that the race demographics of these 
attendance zones often don't reflect the demographics of the overal region (district, city). These "segregated" school attendance zones are a consequence of segregated
neighborhoods in districts and cities. Therefore, this program utilizes an algorithm that combines US census data of block groups (https://en.wikipedia.org/wiki/Census_block_group)
and the partitioning of a region in a similar way to that of a Vornoi diagram (https://en.wikipedia.org/wiki/Voronoi_diagram) in order to create school attendance zones that
are not only more desgregated but also satisfy the general requirements of being comparable in population, contiguous, and generally compact. 


The algorithm first determines which block groups in the targeted region/district are most diverse (reflective of the racial demographics of the overall region). This is determined by a "diversity score," which is calculated
by finding the percent difference between the overall region and the individual block group for each race/ethnic percent composition, then multiplying these percent differences by the corresponding populations of the overall region, and then summing together the resulting
quantities. The block groups with the lowest diversity scores are then the most reflective of the demographics of the overall region. This step is important because
it helps narrow down the number of block groups that we can simulate being the centers of the attendance zones. While the optimal method would be to test every combination
of block groups to be the centers of the attendance zones, this method is impractical because an average city district contains around 1000 census blocks and 10 
school attendance zones, leading to (1000 choose 10) different simulations.

Once the program has found the most diverse block groups (around top 10%), the program will then run simulations creating attendance zones around a set number of 
block groups randomly chosen from this condensed list. Through research and my own testing, I have found a signficant improvement in the diversity of the resulting
attendance zones when first restricitng the attendance zones to be centered around these more diverse block groups; this can be explained by the frequent occurrence
of diverse block groups situated between two more segregated regions. 

In order to create the attendance zones around the preselcted block groups, which are referred to as center block groups or center nodes, a partitioning method inspired
by the Vornoi diagram was used. Each remaining block group is assigned to its closest center node (using US census longitude and latitude numbers), creating paritions
of the overall region that are contiguous and generally compact. Addiotionally, because this method assigns each block group to its closet center node, it helps
minimize the travel time required for each student in the attendance zone. Each center block group and the other block groups that are assigned to it then form an attendance zone.

After running through many different simulations where different combinations of diverse block groups are chosen to be the center nodes, the program then finds the
configurations that lead to attendance zones with the most reflective racial/ethnic demographics and comparable populations. These configurations are then returned
to the user. 

In order to run this program, download all of the files and open the Redistricting.pro file in Qtcreator. A sample prompt of the terminal ouput is shown in the 
Terminal_Sample_Output.txt file. When prompted for the region name, provide a 1 word name. The number of attendance zones should be around 10 for every 1000 census blocks; the
actual number depends on the specific district/region. The input data file must be in the exact format shown in the sample file MaricopaCountyData.txt. This data contains
the Geo-ID, name, population racial demographics, longitude, and latitude for each desired block group. The data was obtained from the following US Census databases:
https://www.census.gov/geographies/reference-files/time-series/geo/centers-population.html, https://data.census.gov/cedsci/. In order to match the data to the required format,
a sample CSV file (MaricopaCountyData.csv) is also provided. When selecting how many simulations to run, it is important to note that more simulations leads
to a higher possibility of finding a more diverse school attendance zone configuration; however, this also requries more runtime. 

The current code only accounts for "white" and "black or African American" demographics. However for any region where other race/ethnicities are a significant
demogrpahic, the code can easily be edited to add in these changes.

Algorithm and all code was developed/written by Raymond Suo. For any other questions on how to use or run this project, please contact Raymond Suo at rms7988@gmail.com

