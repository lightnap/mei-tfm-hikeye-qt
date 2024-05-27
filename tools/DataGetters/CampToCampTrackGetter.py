
from asyncio.windows_events import NULL
import requests 
import json
import time
import os

# Global variables. 
BOUNDS_FILE_PATH = 'Configuration/LocationConfig.json';
DATA_FOLDER_PATH = 'GroundTruthData/CampToCamp/Tracks/';
MAX_ROUTES_TO_DOWNLOAD = 100000;

GET_OUTINGS_ACTION_ID = 1;
DOWNLOAD_OUTINGS_ACTION_ID = 2;
ALL_ACTION_TRACK_ID = 3;

FORCE_GET_OUTINGS_ACTION_ID = 4;
FORCE_DOWNLOAD_OUTINGS_ACTION_ID = 5;
FORCE_ALL_ACTION_TRACK_ID = 6;

FILTER_TIMESTAMPED_TRACKS_ACTION_ID = 7;

PRINT_HELP_ACTION_ID = 8;
QUIT_ACTION_ID = 9; 


# Prints the message to inform the user of the possible actions. 
def PrintInitialMessage(): 

    WAIT_TIME_BETWEEN_PRINTS = 0.075;

    ActionDescriptions = {
        GET_OUTINGS_ACTION_ID : 'Search for all Outings',
        DOWNLOAD_OUTINGS_ACTION_ID : 'Downloaded found Outings if they have a track',
        ALL_ACTION_TRACK_ID : 'Perform actions ' + str(GET_OUTINGS_ACTION_ID) + ' and ' + str(DOWNLOAD_OUTINGS_ACTION_ID),
        FORCE_GET_OUTINGS_ACTION_ID : 'FORCE - Search for all Outings',
        FORCE_DOWNLOAD_OUTINGS_ACTION_ID : 'FORCE - Downloaded found Outings if they have a track',
        FORCE_ALL_ACTION_TRACK_ID : 'FORCE - Perform actions ' + str(FORCE_GET_OUTINGS_ACTION_ID) + ' and ' + str(FORCE_DOWNLOAD_OUTINGS_ACTION_ID),
        FILTER_TIMESTAMPED_TRACKS_ACTION_ID : 'Scan folder of downloaded paths and copy timestamped ones into new folder',
        PRINT_HELP_ACTION_ID : 'Help',
        QUIT_ACTION_ID :'Quit program'
    };

    InitialMessages = [
        '----------------------------------------------------------------------------------------------------------\n'
        '----------------------------------------------------------------------------------------------------------\n'
        'Welcome to CampToCamp track data getter.',
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


# Turns user input to actions. 
def ParseUserActions(aUserInput): 

    SingleActionsIDs = [
        str(GET_OUTINGS_ACTION_ID),
        str(DOWNLOAD_OUTINGS_ACTION_ID),
        
        str(FORCE_GET_OUTINGS_ACTION_ID),
        str(FORCE_DOWNLOAD_OUTINGS_ACTION_ID),
        str(FILTER_TIMESTAMPED_TRACKS_ACTION_ID),

        str(PRINT_HELP_ACTION_ID),
        str(QUIT_ACTION_ID)
    ];

    MultiActionIDs = {
        str(ALL_ACTION_TRACK_ID) : [GET_OUTINGS_ACTION_ID, DOWNLOAD_OUTINGS_ACTION_ID],
        str(FORCE_ALL_ACTION_TRACK_ID) : [FORCE_GET_OUTINGS_ACTION_ID, FORCE_DOWNLOAD_OUTINGS_ACTION_ID]
    }; 

    ActionsToPerform = []; 

    if aUserInput in SingleActionsIDs:

        ActionsToPerform.append(int(aUserInput));

    elif aUserInput in (MultiActionIDs):

        ActionsToPerform += MultiActionIDs[aUserInput]; 

    else:
        print('');
        print('Unrecognized action input. Please try again.');
        print('');
        time.sleep(2); 

    return ActionsToPerform;


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


# Requests the api for the outings. 
def GetFromApiOutings(aForce, aInitialOffset):

    MAX_OUTINGS_PER_PAGE = 100;

    OutingsIdList = [];
    NumberOfRoutesInPage = 1; 
    StatusCode = 200; 

    while StatusCode == 200 and len(OutingsIdList) < MAX_ROUTES_TO_DOWNLOAD:

        OutingsIdUrl = 'https://api.camptocamp.org/routes?act=hiking&limit=' + str(MAX_OUTINGS_PER_PAGE) + '&offset=' + str(len(OutingsIdList) + aInitialOffset);

        Session = requests.session();
        Response = Session.get(OutingsIdUrl, stream=True);
        StatusCode = Response.status_code;
        print('Getting next ' + str(MAX_OUTINGS_PER_PAGE) + ' tracks with offset ' + str(len(OutingsIdList) + aInitialOffset));

        print (StatusCode);
        time.sleep(1);

        if StatusCode == 200:

            OutingsJsonList = Response.json();

            for Outings in OutingsJsonList['documents']:

                OutingsIdList.append(Outings['document_id']);
                print('OutingId: ' + str(Outings['document_id']));

    time.sleep(2);
    return OutingsIdList;


#Obtains outings Id and saves it on file. 
def GetOutings(aForce):

    OUTINGS_LIST_CONFIG_FILE_PATH = 'TrackData/CampToCamp/OutingsToScrap.json'

    print('Getting Outings with Force = ' + str(aForce));

    #Get data from files.
    with open(OUTINGS_LIST_CONFIG_FILE_PATH, 'r') as JsonOutingsFile:
        JsonOutingsToScrapData = json.load(JsonOutingsFile);

    #Reset file if force.
    if aForce:
        JsonOutingsToScrapData = {}; 

    # Obtain data. 
    FoundRoutes = len(JsonOutingsToScrapData); 
    OutingsIdList = GetFromApiOutings(aForce, FoundRoutes);

    for OutingId in OutingsIdList:
        JsonOutingsToScrapData[OutingId] = False; 

    # Update files. 
    with open(OUTINGS_LIST_CONFIG_FILE_PATH, 'w') as JsonOutingsFile:
        json.dump(JsonOutingsToScrapData, JsonOutingsFile);


# Downloads a single outing from the API. 
def DownloadOutingFromAPI(aOutingId): 

    TRACK_DATA_FILE_PATH = 'TrackData/CampToCamp/Tracks/' + str(aOutingId) + '.json';
    OutingDownloadUrl = 'https://api.camptocamp.org/routes/' + str(aOutingId);

    print('Downloading Outing ' + str(aOutingId));

    Session = requests.session();
    Response = Session.get(OutingDownloadUrl, stream=True);
    StatusCode = Response.status_code;

    print (StatusCode);
    time.sleep(1);

    if StatusCode == 200:

        OutingData = Response.json();

        if OutingData['geometry']['geom_detail'] != None:

            print('Outing ' + aOutingId + ' HAS recorded track data! Writing to file...');
            TrackPoints = OutingData['geometry']['geom_detail'];
            TrackDataFile = open(TRACK_DATA_FILE_PATH, 'w');
            TrackDataFile.write(TrackPoints);
            TrackDataFile.close();

        else:
            print('Outing ' + aOutingId + ' has no recorded track data');



#Downloads all outings and saves them on files. 
def DownloadOutings(aForce):

    OUTINGS_LIST_CONFIG_FILE_PATH = 'TrackData/CampToCamp/OutingsToScrap.json'

    print('Downloading outings with Force = ' + str(aForce));

    #Get data from files.
    with open(OUTINGS_LIST_CONFIG_FILE_PATH, 'r') as JsonOutingsFile:
        JsonOutingsToScrapData = json.load(JsonOutingsFile);

    #Reset file if force.
    if aForce: 
        for OutingId in JsonOutingsToScrapData:
            JsonOutingsToScrapData[OutingId] = False;

    for OutingId in JsonOutingsToScrapData:

        if JsonOutingsToScrapData[OutingId] == False:

            DownloadOutingFromAPI(OutingId);
            JsonOutingsToScrapData[OutingId] = True 

    # Update files. 
    with open(OUTINGS_LIST_CONFIG_FILE_PATH, 'w') as JsonOutingsFile:
        json.dump(JsonOutingsToScrapData, JsonOutingsFile);

    time.sleep(2);


#  Scan folder of downloaded paths and copy timestamped ones into new folder.
def FilterTimestampedTracs():

    TRACK_FILES_DIRECTORY = 'TrackData/CampToCamp/Tracks/'; 
    TIMESTAMPED_TRACK_FILES_DIRECTORY = 'TrackData/CampToCamp/TracksWithTimeStamps';

    print('Filtering timestamped tracks');

    for filename in os.scandir(TRACK_FILES_DIRECTORY):
        if filename.is_file():
            print('Determining timestampedness of route' + str(filename.name));

           #Get data from file.
            with open(filename.path, 'r') as JsonTrackFile:
                JsonTrackData = json.load(JsonTrackFile);

            TrackFirstPoint = JsonTrackData['coordinates'][0]; 

            HasTimesTamps = False; 

            # In case we have a list of segments of coordintes. 
            if(type(TrackFirstPoint[0]) != float):
                TrackFirstPoint = JsonTrackData['coordinates'][0][0];
                
            print("Length of the first point " + str(len(TrackFirstPoint)));

            if(len(TrackFirstPoint) == 4 and TrackFirstPoint[3] > 1) or (len(TrackFirstPoint) == 3 and TrackFirstPoint[2] > 7000):
                HasTimesTamps = True; 

            if HasTimesTamps:
                print("Outing  " + filename.name + " HAS timestamps. Saving...");

                # Update files. 
                with open(TIMESTAMPED_TRACK_FILES_DIRECTORY + '/' + filename.name, 'w') as JsonTimestampsTrackFile:
                    json.dump(JsonTrackData, JsonTimestampsTrackFile);

            else: 

                print("Outing  " + filename.name + " has NO timestamps");

            
            #Determine if it has timestamps. 

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

        if GET_OUTINGS_ACTION_ID in aActionsToPerform: 

            GetOutings(False);

        if FORCE_GET_OUTINGS_ACTION_ID in aActionsToPerform:

            GetOutings(True);

        if DOWNLOAD_OUTINGS_ACTION_ID in aActionsToPerform:

            DownloadOutings(False); 

        if FORCE_DOWNLOAD_OUTINGS_ACTION_ID in aActionsToPerform:

            DownloadOutings(True); 

        if FILTER_TIMESTAMPED_TRACKS_ACTION_ID in aActionsToPerform:

            FilterTimestampedTracs(); 


def main():

    ActionsToPerform = [];

    while QUIT_ACTION_ID not in ActionsToPerform:
        ActionsToPerform = SelectActions(); 
        PerformActions(ActionsToPerform); 


if __name__ == "__main__": main()