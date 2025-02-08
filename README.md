# canterbury1940

A holiday winter season project over two weeks in late December 2024 to early January 2025.

### Aim

The aim was to take a PNG map file slightly smaller than:

[Bigger map](https://gitlab.com/apesdk/simulatedape/-/blob/coregraphics2022/mapedit/l40-examples/new_canterbury.png?ref_type=heads)

And make a JSON version of it with a distinctly higher fidelity than the map creating method used for London1940 previously.

The JSON file is central to the simulation of Canterbury in 1940. This JSON is the basis of simulating London 1940 as well (as it will include Canterbury).

Once Canterbury 1940 has been validated as a superior PNG to JSON algorithm it will be reintegrated potentially or may be used to build a small simulation of Canterbury. 

### Additional unneeded data

Obviously the map contains a bunch of additional noise data including text and symbols which should be removed and replaced with some data which will be useful for the simulation of this space.

## Generative AI

As an experiment about sixty percent of the code is being generated with DeepSeek R1 to see how similar this is to having an actual human co-developer exerting peer pressure in the development. This will be discussed to a greater detail once more development is done.

For now, the created source code is similar but not ideally compatible with the ApeSDK. The intention is to integrate this into a shared interface where many of the simplifications from Canterbury1940 can be shared with the ApeSDK.

Generative AI is not a panacea for this kind of development rather than asking the big aspects of the project to be created with Generative AI. It is more about framing small problems which generate substantial code but still requires unification (as there are minimal unified coding practices with Generative AI).

The best example of this: it feels like having a relatively junior but eager open source contributor who write volumes of slightly buggy code which needs to be verified and integrated frequently.

## Getting started

The source code as written here is generic C but contained initially in a slightly Mac centric fashion. It is currently developed as a command line. It would be relatively easy to write a build script for it. Consider that to be a final week objective.

## Background

This project:

* is part of [London1940](https://london1940.org/), 

* which was developed from [ApeSDK](https://apesdk.com/), and, 

* also has been documented in [the Last Monkey Standing Youtube Channel](https://www.youtube.com/@LastMonkeyStanding).

## Blurb

Simulating Canterbury, England, in 1940 presents a unique challenge, as it requires balancing historical accuracy with creative interpretation. Canterbury is a historic city in Kent, known for its iconic cathedral and medieval architecture. In 1940, the city was deeply influenced by its long history and the significant impact of World War II, which shaped daily life.

### Population and Demographics

In 1940, the population of Canterbury was approximately 24,000 people. The city had been relatively small but well-established, with its economy centered around agriculture, trade, education, and tourism related to its historical and religious significance. The University of Kent did not yet exist, but the city hosted schools and other institutions. The population included a mix of residents: working-class families, clergy, shopkeepers, artisans, and civil servants, along with a transient population of soldiers and wartime evacuees from larger cities like London.

#### Key Details for a Simulation

##### Historical and Architectural Landscape

Canterbury Cathedral, a UNESCO World Heritage Site, was the centerpiece of the city.
Other landmarks included the Norman-era Canterbury Castle, the city walls, and Westgate, a medieval gatehouse.
The streets were lined with Tudor and Georgian buildings, many of which housed shops, pubs, and homes.
Transportation and Infrastructure:

The city's infrastructure revolved around narrow, cobbled streets with limited motorized traffic.
Railways connected Canterbury to other parts of Kent and London, while local buses and bicycles were common for short-distance travel.

##### Daily Life

Rationing was a part of everyday life, with food, clothing, and fuel strictly controlled.
The city’s economy relied on small businesses, markets, and services, with limited industrial presence.
The social life revolved around pubs, churches, and community events.

##### Wartime Context

In 1940, Canterbury was within the range of German air raids due to its proximity to the English Channel. The "Baedeker Blitz" heavily impacted the city in later years, but the threat of invasion and bombings influenced life even in 1940.
The presence of the military and civil defense organizations, such as the Home Guard, was notable.

Air raid precautions (ARP) included blackout regulations, air raid shelters, and regular drills.

##### Excluding World War II

Simulating Canterbury without including World War II would remove a significant aspect of its 1940s context. However, focusing solely on the city’s historical and cultural identity is feasible.

The simulation emphasizes:

The architectural and medieval charm of the city.
The daily routines of its residents in peacetime.
Religious pilgrimages to Canterbury Cathedral, drawing inspiration from Geoffrey Chaucer’s The Canterbury Tales.
This alternate approach would involve ignoring the war's physical and psychological effects, such as bomb damage, rationing, and wartime migration. Instead, the simulation could reconstruct the city as an idyllic setting, emphasizing traditional English life, historic trade, and cultural activities.

In conclusion, while it is possible to simulate Canterbury without including World War II, doing so would sacrifice an essential dimension of its 1940 identity. The war's impact on infrastructure, social structures, and the city’s role in broader national events defines much of its historical significance during this period. Balancing these aspects is key to creating a compelling and authentic simulation.

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[ApeSDK License](https://gitlab.com/apesdk/apesdk/-/blob/coregraphics2022/LICENSE?ref_type=heads)
