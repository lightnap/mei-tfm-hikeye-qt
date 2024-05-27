
import requests 
import json
import time
import xml.etree.ElementTree as ET
import os

# Global variables. 
REGIONS_FILE_PATH = 'Configuration/LocationConfig.json';

DOWNLOAD_TRACES_ACTION_ID = 1;
FORCE_DOWNLOAD_TRACES_ACTION_ID = 2; 
DOWNLOAD_GROUND_TRUTH_ACTION_ID = 3; 
FORCE_DOWNLOAD_GROUND_TRUTH_ACTION_ID = 4;
PRINT_HELP_ACTION_ID = 5;
QUIT_ACTION_ID = 6; 


# Prints the message to inform the user of the possible actions. 
def PrintInitialMessage(): 

    WAIT_TIME_BETWEEN_PRINTS = 0.075;

    ActionDescriptions = {
        DOWNLOAD_TRACES_ACTION_ID : 'Download GPS traces of a given region',
        FORCE_DOWNLOAD_TRACES_ACTION_ID : 'FORCE - Download GPS traces of a given region',
        DOWNLOAD_GROUND_TRUTH_ACTION_ID : 'Download ground truth of a given region',
        FORCE_DOWNLOAD_GROUND_TRUTH_ACTION_ID : 'FORCE - Download ground truth of a given region',
        PRINT_HELP_ACTION_ID : 'Help',
        QUIT_ACTION_ID :'Quit program'
    };

    InitialMessages = [
        '----------------------------------------------------------------------------------------------------------\n'
        '----------------------------------------------------------------------------------------------------------\n'
        'Welcome to the OpenStreetMap track data getter.',
        'Please select which action you would like to perform.',
        'Please select only one number and press enter.',
        '----------------------------------------------------------------------------------------------------------',
        'Possible actions:'
    ];

    FinalMessages = [
        '----------------------------------------------------------------------------------------------------------',
        'Please select your desired action'
    ];

    for Message in InitialMessages:
        print(Message);
        time.sleep(WAIT_TIME_BETWEEN_PRINTS);

    for MessageId in ActionDescriptions:
        print ('(' + str(MessageId) + ') - ' + ActionDescriptions[MessageId]);
        time.sleep(WAIT_TIME_BETWEEN_PRINTS); 

    for Message in FinalMessages:
        print(Message);
        time.sleep(WAIT_TIME_BETWEEN_PRINTS);

    print('');


def PrintSelectRegionMessage(aRegionsNamesList): 

    WAIT_TIME_BETWEEN_PRINTS = 0.075;

    InitialMessages = [
        '\n\n'
        '----------------------------------------------------------------------------------------------------------\n'
        '----------------------------------------------------------------------------------------------------------\n'
        'Please select a region from which to obtain data.',
        'Please select only one number and press enter.',
        '----------------------------------------------------------------------------------------------------------',
        'Possible regions:'
    ];

    RegionsMessages = []; 

    for RegionNumber in range(0, len(aRegionsNamesList)):
        RegionsMessages.append('(' + str(RegionNumber + 1) + ') - ' + aRegionsNamesList[RegionNumber]);

    RegionsMessages.append('(' + str( len(aRegionsNamesList) + 1) + ') - ' + 'Back to action menu'); 

    FinalMessages = [
        '----------------------------------------------------------------------------------------------------------',
        'Please select a region'
    ];

    TotalMessages = InitialMessages + RegionsMessages + FinalMessages; 

    for Message in TotalMessages:
        print(Message);
        time.sleep(WAIT_TIME_BETWEEN_PRINTS);



# Turns user input to actions. 
def ParseUserActions(aUserInput): 

    SingleActionsIDs = [
        str(DOWNLOAD_TRACES_ACTION_ID),
        str(FORCE_DOWNLOAD_TRACES_ACTION_ID),
        str(DOWNLOAD_GROUND_TRUTH_ACTION_ID),
        str(FORCE_DOWNLOAD_GROUND_TRUTH_ACTION_ID),

        str(PRINT_HELP_ACTION_ID),
        str(QUIT_ACTION_ID)
    ];


    ActionsToPerform = []; 

    if aUserInput in SingleActionsIDs:

        ActionsToPerform.append(int(aUserInput));

    else:
        print('');
        print('Unrecognized action input. Please try again.');
        print('');

    return ActionsToPerform;


# Turns user input to string with region name.
def ParseUserRegion(aRegionsList, UserInput):

    IntegerList = [str(x) for x in range(1, len(aRegionsList) + 1)];

    if UserInput in IntegerList:
       return aRegionsList[int(UserInput) - 1];

    elif UserInput == str(len(aRegionsList) + 1):

        return 'Back';

    else:
        print('');
        print('Unrecognized action input. Please try again.');
        print('');
        time.sleep(1); 
        return None; 


# Accepts user input and decides actions to perform based on that. 
def SelectActions():

    ActionsToPerform = []; 

    while not ActionsToPerform: 

        PrintInitialMessage(); 

        UserInput = input(); 

        ActionsToPerform = ParseUserActions(UserInput); 

    print('Will perform the following actions:');
    print('');
    print (ActionsToPerform);
    print('');
    print('');
    time.sleep(1); 

    return ActionsToPerform; 


# Allows user to select region to download data from. 
def SelectRegion(aRegionsConfig): 
   
    SelectedRegion = None; 

    RegionsList = []; 
    RegionsNames = []; 

    for Region in aRegionsConfig:
        RegionsList.append(Region);
        RegionsNames.append(aRegionsConfig[Region]['Name']);

    while not SelectedRegion:

        PrintSelectRegionMessage(RegionsNames);
        UserInput = input();
        SelectedRegion = ParseUserRegion(RegionsList, UserInput);
    
    if SelectedRegion == 'Back':
        print('\n Returning to actions menu...')

    else: 
        print('Will download the following region: \n \n');
        print ('[' + aRegionsConfig[SelectedRegion]['Name'] + ']');
        print('\n\n');

    time.sleep(1); 

    return SelectedRegion;


# Reads the bounding box data stored in the corresponding json file.
def ObtainJsonRegionsList():

    JsonFile = open(REGIONS_FILE_PATH);
    JsonData = json.load(JsonFile);

    RegionsList = JsonData['RegionsByBoundingBox'];

    return RegionsList; 


def SaveTraceToFile(aTrack, aPreviousTrackId, aRegion, aNameSpacePrefixMap):

    TrackId = aTrack.find('url', aNameSpacePrefixMap).text.replace('/', '_');
    FILENAME = 'TrackData/OpenStreetMap/' + aRegion + '/' + TrackId + '.gpx';

    # Track is different from previous one. 
    if TrackId != aPreviousTrackId:

        # XMLHeading = '\n <?xml version="1.0" encoding="UTF-8"?> \n';
        #GPXHeading = '<gpx version="1.0" creator="OpenStreetMap.org" xmlns="http://www.topografix.com/GPX/1/0"> \n'

        XMLTrackString = ET.tostring(aTrack, encoding='utf8', method='xml');

        with open(FILENAME, 'wb') as TrackFile:
            TrackFile.write(XMLTrackString);

    # Track was same as previous, got cut between pages. 
    else: 

        print('Track ' + TrackId + ' is same as previous. Appending...'); 

        Parser = ET.XMLParser(encoding="utf-8")

        ExistingXMLTrackTree = ET.parse(FILENAME, parser = Parser);
        ExistingXMLTrackTreeRoot = ExistingXMLTrackTree.getroot(); 
        ExistingXMLTrkSkElement = ExistingXMLTrackTreeRoot.find('trkseg', aNameSpacePrefixMap);

        ToAppendTrackSegment = aTrack.find('trkseg', aNameSpacePrefixMap);

        for TrackPointToAppend in ToAppendTrackSegment.findall('trkpt', aNameSpacePrefixMap):
            ExistingXMLTrkSkElement.append(TrackPointToAppend);

        XMLTrackString = ET.tostring(aTrack, encoding='utf8', method='xml');

        with open(FILENAME, 'wb') as TrackFile:

            TrackFile.write(XMLTrackString);

    return TrackId;


# Download GPS traces of a given bounding box. 
def DownloadTraces(aBoundingBox, aRegion, aForce):

    print('Downloading traces of area ' + aRegion + ' and force = ' + str(aForce)); 

    TRACES_FOLDER = "TrackData/OpenStreetMap/" + aRegion + "/";

    #Create directory if it doesnt exist. 
    if not os.path.exists(TRACES_FOLDER):
        os.mkdir(TRACES_FOLDER);

    # Delete all files in folder.
    for Filename in os.listdir(TRACES_FOLDER):
        Filepath = os.path.join(TRACES_FOLDER, Filename);
        if os.path.isfile(Filepath):
            os.unlink(Filepath);

    VisitedAllPages = False;
    CurrentPageNumber = 0;
    PreviousTrackId = None;
    BoundingBoxString = 'bbox=' + str(aBoundingBox[3]) + ',' + str(aBoundingBox[1]) + ',' + str(aBoundingBox[2]) + ',' + str(aBoundingBox[0]);
    NameSpacePrefixMap = {'':'http://www.topografix.com/GPX/1/0'};

    Session = requests.session();


    while not VisitedAllPages:

        TracksUrl = 'https://api.openstreetmap.org/api/0.6/trackpoints?' + BoundingBoxString + '&page=' + str(CurrentPageNumber);

        Response = Session.get(TracksUrl, stream=True);
        StatusCode = Response.status_code;

        print("Response status from GET on page " + str(CurrentPageNumber) + ": " + str(StatusCode));

        time.sleep(1); 

        PageOfTracksXml = ET.fromstring(Response.content);

        if PageOfTracksXml.find('trk', NameSpacePrefixMap) == None:

            VisitedAllPages = True

        else:

            for Track in PageOfTracksXml.findall('trk', NameSpacePrefixMap):

                if (Track.find('name', NameSpacePrefixMap) != None) or (Track.find('desc', NameSpacePrefixMap) != None) or (Track.find('url', NameSpacePrefixMap) != None):

                    PreviousTrackId = SaveTraceToFile(Track, PreviousTrackId, aRegion, NameSpacePrefixMap);

            CurrentPageNumber += 1;


#Construct overpass query for a given bounding box. 
def ConstructGroundTruthQuery(aBoundingBox):

    BoundingBoxString ='(' + aBoundingBox[1] + ',' + aBoundingBox[3] + ',' + aBoundingBox[0] + ',' + aBoundingBox[2] + ')';

    SearchTracks = 'nwr["highway"="track"]' + BoundingBoxString + ';';
    SearchPaths = 'nwr["highway"="path"]' + BoundingBoxString + ';';

    QueryStart = 'data=('
    QueryEnd = ');out geom;'

    Query = QueryStart + SearchPaths + SearchTracks + QueryEnd;
    return Query;


# Download GroundTruth of a given bounding box. 
def DownloadGroundTruth(aBoundingBox, aRegion, aForce):

    print('Downloading ground truth of area' + aRegion + ' and force = ' + str(aForce)); 

    GROUND_TRUTH_FILE_PATH = 'GroundTruthData/' + aRegion + '.gpx';

    Query = ConstructGroundTruthQuery(aBoundingBox);

    Webpage = 'http://overpass-api.de/api/interpreter?'
    RequestedData = requests.get(Webpage + Query);

    with open(GROUND_TRUTH_FILE_PATH, 'w') as GroundTruthFile:
        GroundTruthFile.write(RequestedData.text);


#PrintsHelps. 
def PrintHelp():
    print('----------------------------------------------------------------------------------------------------------');
    print('');
    print("#TODO: Write some Helps");
    print('');
    print('----------------------------------------------------------------------------------------------------------');


# Perform actions needed before quitting. 
def Quit():
    print('');
    print('Buh-bye!');
    print('');

# Calls the appropiate functions according to the parameter. 
def PerformActions(aActionsToPerform): 

    if QUIT_ACTION_ID in aActionsToPerform:

        Quit(); 

    elif PRINT_HELP_ACTION_ID in aActionsToPerform:

        PrintHelp(); 

    else:

        RegionsList = ObtainJsonRegionsList();
        RegionToDownload = SelectRegion(RegionsList);

        if RegionToDownload != 'Back':

            BoundingBox = RegionsList[RegionToDownload]["BoundingBox"]; 

            if DOWNLOAD_TRACES_ACTION_ID in aActionsToPerform:
        
                DownloadTraces(BoundingBox, RegionToDownload, False); 

            if FORCE_DOWNLOAD_TRACES_ACTION_ID in aActionsToPerform:

                DownloadTraces(BoundingBox, RegionToDownload, True); 

            if DOWNLOAD_GROUND_TRUTH_ACTION_ID in aActionsToPerform:

                DownloadGroundTruth(BoundingBox, RegionToDownload, False);

            if FORCE_DOWNLOAD_GROUND_TRUTH_ACTION_ID in aActionsToPerform:

                DownloadGroundTruth(BoundingBox, RegionToDownload, True);


# Downloads tracks from configured region using Gaia GPS. 
def main():

    ActionsToPerform = [];

    while QUIT_ACTION_ID not in ActionsToPerform:
        ActionsToPerform = SelectActions(); 
        PerformActions(ActionsToPerform); 


if __name__ == "__main__": main()