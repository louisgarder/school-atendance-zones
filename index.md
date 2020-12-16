### What are school attendance zones and what is the purpose of this project?

School attendance zones are defined as the outlines in a school district where students will attend public schools. General requirements for schoool attendance zones are contiguity and compactness in shape in order to minimize travel distances for students. A major problem with current school attendance zones is that they are often created without consideration of the racial/ethnic demographics in zones; Because many regions in the US are still segregated, this consequently leads to the creation of segregated school attendane zones and segregated classrooms. **The purpose of this project is to create school attendance zones for a region that have racial/ethnic demographics that beter reflect the demographics of the overall region while still mantaining contiguity and compactness.**  

### Overview of the project

In order to create more desegregated school attendance zones, this project is one of the first computer programs to utilize US demographic data of Census Block Groups, the smallest geographical unit for which the bureau publishes sample data. In a completely novel algorithm, the program first determines the census blocks in the region that are the most desegregated and then uses these more diverse areas as the center nodes of the attendance zones. Each remaining census block is then matched to its closest center node, creating attendance zones that are compact and contiguous. Because the attendance zones are centered around desegregated census blocks, which are often situated between segregated areas, the zones are consequently able to combine together different demographics into a more diverse classroom. 

<<img width="433" alt="Screen Shot 2020-12-15 at 10 29 39 PM" src="https://user-images.githubusercontent.com/32240284/102301593-1561c180-3f25-11eb-82a4-f554d9cf4f2d.png"> Before 

