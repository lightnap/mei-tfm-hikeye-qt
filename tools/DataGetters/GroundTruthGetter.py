
import requests
import json


# Global variables. 
BOUNDS_FILE_PATH = 'Configuration/LocationConfig.json';
GROUND_TRUTH_FILE_PATH = 'GroundTruthData/test.xml';

# Reads the bounding box data stored in the corresponding json file.
def ObtainJsonBoundsData():
	JsonFile = open(BOUNDS_FILE_PATH);
	JsonData = json.load(JsonFile);

	BoundingBox = JsonData['BoundingBox'];
	return BoundingBox; 


# Constructs the Overpass query.
def ConstructQuery(aBoundingBox):
	
	# Example queries that we know work. 
	#query = 'data=node["amenity"="drinking_water"](41.380978689378,2.1469688415527,41.395403000626,2.1696066856384);out;'
	#query = 'data=(nwr["highway"="path"](42.19838506744,2.3436069488525,42.255331895816,2.4341583251953);nwr["highway"="track"](42.19838506744,2.3436069488525,42.255331895816,2.4341583251953););out geom;'

	NorthLimit = aBoundingBox['NorthLimit'];
	SouthLimit = aBoundingBox['SouthLimit'];
	EastLimit = aBoundingBox['EastLimit'];
	WestLimit = aBoundingBox['WestLimit'];

	BoundingBoxString ='(' + SouthLimit + ',' + WestLimit + ',' + NorthLimit + ',' + EastLimit + ')';

	SearchTracks = 'nwr["highway"="track"]' + BoundingBoxString + ';';
	SearchPaths = 'nwr["highway"="path"]' + BoundingBoxString + ';';

	QueryStart = 'data=('
	QueryEnd = ');out geom;'

	Query = QueryStart + SearchPaths + SearchTracks + QueryEnd;
	return Query


def DownloadGroundTruth(): 

	BoundingBox = ObtainJsonBoundsData(); 

	Query = ConstructQuery(BoundingBox);

	Webpage = 'http://overpass-api.de/api/interpreter?'

	# http://overpass-api.de/api/interpreter?data=(nwr["highway"="path"](42.19,2.34,42.26,2.43);nwr["highway"="track"](42.19,2.34,42.26,2.43););out geom;
	RequestedData = requests.get(Webpage + Query);

	print(RequestedData.text);

	GroundTruthFile = open(GROUND_TRUTH_FILE_PATH, 'w');
	GroundTruthFile.write(RequestedData.text);
	GroundTruthFile.close(); 


def main(): 
	DownloadGroundTruth();


if __name__ == '__main__': main()
