import json

gc_cities = {}
gc_countries = {}
cities = {}


stopwords = set(['the', 'of', 'district', 'new', 'old', 'north', 'south', 'west', 'east', 'west'])
gc_countries = json.loads(open("countries.json").read())
gc_cities = json.loads(open("cities.json").read())
gc_us_states = {
    'AK': {'code': 'AK', 'name': 'Alaska', 'geonameid': 5879092},
    'AL': {'code': 'AL', 'name': 'Alabama', 'geonameid': 4829764},
    'AR': {'code': 'AR', 'name': 'Arkansas', 'geonameid': 4099753},
    'AZ': {'code': 'AZ', 'name': 'Arizona', 'geonameid': 5551752},
    'CA': {'code': 'CA', 'name': 'California', 'geonameid': 5332921},
    'CO': {'code': 'CO', 'name': 'Colorado', 'geonameid': 5417618},
    'CT': {'code': 'CT', 'name': 'Connecticut', 'geonameid': 4831725},
    'DC': {'code': 'DC', 'name': 'District of Columbia', 'geonameid': 4138106},
    'DE': {'code': 'DE', 'name': 'Delaware', 'geonameid': 4142224},
    'FL': {'code': 'FL', 'name': 'Florida', 'geonameid': 4155751},
    'GA': {'code': 'GA', 'name': 'Georgia', 'geonameid': 4197000},
    'HI': {'code': 'HI', 'name': 'Hawaii', 'geonameid': 5855797},
    'IA': {'code': 'IA', 'name': 'Iowa', 'geonameid': 4862182},
    'ID': {'code': 'ID', 'name': 'Idaho', 'geonameid': 5596512},
    'IL': {'code': 'IL', 'name': 'Illinois', 'geonameid': 4896861},
    'IN': {'code': 'IN', 'name': 'Indiana', 'geonameid': 4921868},
    'KS': {'code': 'KS', 'name': 'Kansas', 'geonameid': 4273857},
    'KY': {'code': 'KY', 'name': 'Kentucky', 'geonameid': 6254925},
    'LA': {'code': 'LA', 'name': 'Louisiana', 'geonameid': 4331987},
    'MA': {'code': 'MA', 'name': 'Massachusetts', 'geonameid': 6254926},
    'MD': {'code': 'MD', 'name': 'Maryland', 'geonameid': 4361885},
    'ME': {'code': 'ME', 'name': 'Maine', 'geonameid': 4971068},
    'MI': {'code': 'MI', 'name': 'Michigan', 'geonameid': 5001836},
    'MN': {'code': 'MN', 'name': 'Minnesota', 'geonameid': 5037779},
    'MO': {'code': 'MO', 'name': 'Missouri', 'geonameid': 4398678},
    'MS': {'code': 'MS', 'name': 'Mississippi', 'geonameid': 4436296},
    'MT': {'code': 'MT', 'name': 'Montana', 'geonameid': 5667009},
    'NC': {'code': 'NC', 'name': 'North Carolina', 'geonameid': 4482348},
    'ND': {'code': 'ND', 'name': 'North Dakota', 'geonameid': 5690763},
    'NE': {'code': 'NE', 'name': 'Nebraska', 'geonameid': 5073708},
    'NH': {'code': 'NH', 'name': 'New Hampshire', 'geonameid': 5090174},
    'NJ': {'code': 'NJ', 'name': 'New Jersey', 'geonameid': 5101760},
    'NM': {'code': 'NM', 'name': 'New Mexico', 'geonameid': 5481136},
    'NV': {'code': 'NV', 'name': 'Nevada', 'geonameid': 5509151},
    'NY': {'code': 'NY', 'name': 'New York', 'geonameid': 5128638},
    'OH': {'code': 'OH', 'name': 'Ohio', 'geonameid': 5165418},
    'OK': {'code': 'OK', 'name': 'Oklahoma', 'geonameid': 4544379},
    'OR': {'code': 'OR', 'name': 'Oregon', 'geonameid': 5744337},
    'PA': {'code': 'PA', 'name': 'Pennsylvania', 'geonameid': 6254927},
    'RI': {'code': 'RI', 'name': 'Rhode Island', 'geonameid': 5224323},
    'SC': {'code': 'SC', 'name': 'South Carolina', 'geonameid': 4597040},
    'SD': {'code': 'SD', 'name': 'South Dakota', 'geonameid': 5769223},
    'TN': {'code': 'TN', 'name': 'Tennessee', 'geonameid': 4662168},
    'TX': {'code': 'TX', 'name': 'Texas', 'geonameid': 4736286},
    'UT': {'code': 'UT', 'name': 'Utah', 'geonameid': 5549030},
    'VA': {'code': 'VA', 'name': 'Virginia', 'geonameid': 6254928},
    'VT': {'code': 'VT', 'name': 'Vermont', 'geonameid': 5242283},
    'WA': {'code': 'WA', 'name': 'Washington', 'geonameid': 5815135},
    'WI': {'code': 'WI', 'name': 'Wisconsin', 'geonameid': 5279468},
    'WV': {'code': 'WV', 'name': 'West Virginia', 'geonameid': 4826850},
    'WY': {'code': 'WY', 'name': 'Wyoming', 'geonameid': 5843591}
}

us_assoc = ['usa', 'united', 'states', 'america']
uk_assoc = ['united', 'kingdom', 'england', 'uk', 'britain']

for word in us_assoc:
   if (word != 'united'):
      cities[word] = us_assoc

for word in uk_assoc:
   if (word != 'united'):
      cities[word] = uk_assoc

   
   
for city in gc_cities.iteritems():
   cityname = ((city[1])['name']).lower()
   citynames = cityname.split(' ')
#   cities[cityname] = []
   
   if (gc_countries[(city[1])['countrycode']]['name'] == 'United States'):
      for word in citynames:
         cities[word] = us_assoc
      
   elif (gc_countries[(city[1])['countrycode']]['name'] == 'United Kingdom'):
      #cities[cityname] = uk_assoc
      for word in citynames:
         cities[word] = uk_assoc

   else: 
      for word in citynames:
         cities[word] = set(((gc_countries[(city[1])['countrycode']]['name']).lower()).split(' '))
         cities[word] = list((cities[word] - stopwords))

states = []
for state in gc_us_states.iteritems():
   states.extend(((state[1]['name']).lower()).split(' '))

states = set(states) - stopwords
for state in states:
   cities[state] = us_assoc

state_abbreviations = {}
for abbr, state_details in gc_us_states.iteritems():
   state_abbreviations[abbr.lower()] = ((gc_us_states[abbr]['name'].lower()).split()) + us_assoc

for abbr in state_abbreviations.keys():
   cities[abbr] = state_abbreviations[abbr]
   
   

myfile = open("thesaurus.json", "w")
jsondata = json.dumps(cities, indent=4, sort_keys=True)
myfile.write (jsondata)
myfile.close()


   
    
      
   
   
         
      
   
   


   
