
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium import webdriver
import requests 
import json
import time

# Global variables. 
REGIONS_FILE_PATH = 'Configuration/LocationConfig.json';
CHROME_DRIVER_PATH = "C:/Users/roger/Documents/Universitat/TFM/BrowserDrivers/chromedriver.exe";
MAX_ROUTES_PER_REGION = 10000000;

GET_ROUTES_ACTION_ID = 1;
GET_TRACKS_ACTION_ID = 2;
DOWNLOAD_TRACKS_ACTION_ID = 3;
ALL_ACTION_TRACK_ID = 4;
FORCE_GET_ROUTES_ACTION_ID = 5;
FORCE_GET_TRACKS_ACTION_ID = 6;
FORCE_DOWNLOAD_TRACKS_ACTION_ID = 7;
FORCE_ALL_ACTION_TRACK_ID = 8;
PRINT_HELP_ACTION_ID = 9;
QUIT_ACTION_ID = 10; 


# Prints the message to inform the user of the possible actions. 
def PrintInitialMessage(): 

    WAIT_TIME_BETWEEN_PRINTS = 0.075;

    ActionDescriptions = {
        GET_ROUTES_ACTION_ID : 'Search for Routes of configured regions',
        GET_TRACKS_ACTION_ID : 'Search for Tracks of downloaded Routes',
        DOWNLOAD_TRACKS_ACTION_ID : 'Downloaded found Tracks',
        ALL_ACTION_TRACK_ID : 'Perform actions ' + str(GET_ROUTES_ACTION_ID) + ' and ' + str(GET_TRACKS_ACTION_ID) + ' and ' + str(DOWNLOAD_TRACKS_ACTION_ID),
        FORCE_GET_ROUTES_ACTION_ID : 'FORCE - Search for Routes of configured regions',
        FORCE_GET_TRACKS_ACTION_ID : 'FORCE - Search for Tracks of downloaded Routes',
        FORCE_DOWNLOAD_TRACKS_ACTION_ID : 'FORCE - Downloaded found Tracks',
        FORCE_ALL_ACTION_TRACK_ID : 'FORCE - Perform actions ' + str(FORCE_GET_ROUTES_ACTION_ID) + ' and ' + str(FORCE_GET_TRACKS_ACTION_ID) + ' and ' + str(FORCE_DOWNLOAD_TRACKS_ACTION_ID),
        PRINT_HELP_ACTION_ID : 'Help',
        QUIT_ACTION_ID :'Quit program'
    };

    InitialMessages = [
        '----------------------------------------------------------------------------------------------------------\n'
        '----------------------------------------------------------------------------------------------------------\n'
        'Welcome to gaia GPS track data getter.',
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
        str(GET_ROUTES_ACTION_ID),
        str(GET_TRACKS_ACTION_ID),
        str(DOWNLOAD_TRACKS_ACTION_ID),
        
        str(FORCE_GET_ROUTES_ACTION_ID),
        str(FORCE_GET_TRACKS_ACTION_ID),
        str(FORCE_DOWNLOAD_TRACKS_ACTION_ID),

        str(PRINT_HELP_ACTION_ID),
        str(QUIT_ACTION_ID)
    ];

    MultiActionIDs = {
        str(ALL_ACTION_TRACK_ID) : [GET_ROUTES_ACTION_ID, GET_TRACKS_ACTION_ID, DOWNLOAD_TRACKS_ACTION_ID],
        str(FORCE_ALL_ACTION_TRACK_ID) : [FORCE_GET_ROUTES_ACTION_ID, FORCE_GET_TRACKS_ACTION_ID, FORCE_DOWNLOAD_TRACKS_ACTION_ID]
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


# Reads the bounding box data stored in the corresponding json file.
def ObtainJsonRegionsList():

    JsonFile = open(REGIONS_FILE_PATH);
    JsonData = json.load(JsonFile);

    RegionsList = JsonData['GaiaGPSRegions'];

    return RegionsList; 


# Obtains opens a page where you can manually log in to Gaia GPS.
def Login(): 

    LoginUrl = 'https://www.gaiagps.com/login/'

    ChromeService = Service(executable_path = CHROME_DRIVER_PATH)
    Driver = webdriver.Chrome(service = ChromeService);

    Driver.maximize_window();

    Driver.get(LoginUrl);

    time.sleep(1); 

    LoggedInUrl = "https://www.gaiagps.com/map/";

    while not (LoggedInUrl in Driver.current_url):
        print('Waiting For Login');
        time.sleep(1);

    print('LoginSuccessful');

    return Driver; 


# Scraps regions webpages to obtain the obtains the preplanned routes ID.
def ScrapRegion(aCountry, aRegion, aDriver):

    print("Obtaining tracks from " + aCountry + "/" + aRegion);

    PageNumber = 1; 
    MaxRoutesPerPage = 25; 

    NumberOfRoutesInPage = 1; 

    RoutesIdList = []; 

    while NumberOfRoutesInPage > 0 and len(RoutesIdList) < MAX_ROUTES_PER_REGION:
        
        RegionURL = 'https://www.gaiagps.com/hike/' + aCountry + '/' + aRegion + '/?p=' + str(PageNumber) + '&n=' + str(MaxRoutesPerPage) + '#hikes';
        aDriver.get(RegionURL);

        time.sleep(1);

        HikeListElement = aDriver.find_elements(By.XPATH,"//ol[@class='hike-list']/li");

        NumberOfRoutesInPage = len(HikeListElement);

        print("Found " + str(NumberOfRoutesInPage) + " in page " + str(PageNumber));

        RouteNumber = 1; 

        while RouteNumber <= NumberOfRoutesInPage and len(RoutesIdList) < MAX_ROUTES_PER_REGION:

            RouteId = aDriver.find_element(By.XPATH, "//ol[@class='hike-list']/li[" + str(RouteNumber) + "]/div/a").get_attribute("id");
            ShortRouteId = RouteId.replace('hike', '');
            print("Found route Id " + ShortRouteId);
            time.sleep(1);
            RoutesIdList.append(ShortRouteId);
            RouteNumber += 1;

        PageNumber += 1; 

    return RoutesIdList


# Main function for writing routes of region into a file. 
def GetRoutesFromRegion(aCountry, aState, aWebdriver, aForce): 

    REGION_SCRAPPER_CONFIG_FILE_PATH = 'TrackData/GaiaGPS/RegionsToScrap.json';
    ROUTES_SCRAPPER_CONFIG_FILE_PATH = 'TrackData/GaiaGPS/' + aCountry + '/' + aState + '/RoutesToScrap.json'

    #Get data from files.
    with open(REGION_SCRAPPER_CONFIG_FILE_PATH, 'r') as JsonRegionFile:
        JsonRegionToScrapData = json.load(JsonRegionFile);

    with open(ROUTES_SCRAPPER_CONFIG_FILE_PATH, 'r') as JsonRoutesFile:
        JsonRoutesToScrapData = json.load(JsonRoutesFile);

    print("Get routes from " + aCountry + "/" + aState + "and force = " + str(aForce));

    #Append all regions that are not there to the "regions to scrap file", with downloaded = false. 
    if not (aState in JsonRegionToScrapData):
        JsonRegionToScrapData[aState] = False;

    # Reset files if force.
    if aForce:
        JsonRegionToScrapData[aState] = False;
        JsonRoutesToScrapData = {}; 

    # Obtain data
    if JsonRegionToScrapData[aState] == False: 
        ScrappedRoutesId = ScrapRegion(aCountry, aState, aWebdriver);

        for RouteId in ScrappedRoutesId: 
            JsonRoutesToScrapData[RouteId] = False; 

        JsonRegionToScrapData[aState] = True; 

    # Update files. 
    with open(REGION_SCRAPPER_CONFIG_FILE_PATH, 'w') as JsonRegionFile:
        json.dump(JsonRegionToScrapData, JsonRegionFile);

    with open(ROUTES_SCRAPPER_CONFIG_FILE_PATH, 'w') as JsonRoutesFile:
        json.dump(JsonRoutesToScrapData, JsonRoutesFile);


# Scraps a given route and returns a list with its the recorded public tracks.
def ScrapRoute(aRouteId, aDriver):

    TrackIdList = [];

    print('Finding tracks of route ' + str(aRouteId))

    RouteURL = 'https://www.gaiagps.com/hike/' + str(aRouteId);
    aDriver.get(RouteURL);
    time.sleep(1);

    TrackListElementNumber = len(aDriver.find_elements(By.XPATH,"//div[@class='hk-section-content']/div"));

    if TrackListElementNumber <= 0:

        print("No public tracks found for route" + aRouteId);

    else:

        for Track in range(TrackListElementNumber):

            TrackURL = aDriver.find_element(By.XPATH,"//div[@class='hk-section-content']/div[" + str(Track + 1) + "]/div/a").get_attribute('href');

            URLPadding1 = 'https://www.gaiagps.com/datasummary/track/';
            URLPadding2 = 'https://www.gaiagps.com/public/';

            if TrackURL.find(URLPadding1) != -1:
                TrackShortURL =  TrackURL.replace(URLPadding1, '');
            elif TrackURL.find(URLPadding2) != -1:
                TrackShortURL =  TrackURL.replace(URLPadding2, '');

            TrackID = TrackShortURL.replace('/', ''); 

            print ('Found track of route ' + str(aRouteId) + ' with TrackId ' + str(TrackID))

            TrackIdList.append(TrackID);
            time.sleep(1);

    return TrackIdList;


# Main function for writing list of tracks into a file.  
def GetTracksFromRoute(aCountry, aState, aWebdriver, aForce):

    ROUTES_SCRAPPER_CONFIG_FILE_PATH = 'TrackData/GaiaGPS/' + aCountry + '/' + aState + '/RoutesToScrap.json';
    TRACK_DOWNLOAD_CONFIG_FILE_PATH = 'TrackData/GaiaGPS/' + aCountry + '/' + aState + '/TracksToDownload.json';

    #Get data from files.
    with open(ROUTES_SCRAPPER_CONFIG_FILE_PATH, 'r') as JsonRoutesFile:
        JsonRoutesToScrapData = json.load(JsonRoutesFile);

    with open(TRACK_DOWNLOAD_CONFIG_FILE_PATH, 'r') as JsonTracksFile:
        JsonTracksToDownloadData = json.load(JsonTracksFile);

    print("Get tracks from " + aCountry + "/" + aState + "and force = " + str(aForce));

    # Reset files if force.
    if aForce:

        for RouteId in JsonRoutesToScrapData: 

            JsonRoutesToScrapData[RouteId] = False;

        JsonTracksToDownloadData = {}; 

    # Obtain data.
    for RouteId in JsonRoutesToScrapData: 

       if JsonRoutesToScrapData[RouteId] == False: 

           TracksIdList = ScrapRoute(RouteId, aWebdriver);

           for Track in TracksIdList:

               JsonTracksToDownloadData[Track] = False; 

           JsonRoutesToScrapData[RouteId] = True;

    # Update files. 
    with open(ROUTES_SCRAPPER_CONFIG_FILE_PATH, 'w') as JsonRoutesFile:
        json.dump(JsonRoutesToScrapData, JsonRoutesFile);

    with open(TRACK_DOWNLOAD_CONFIG_FILE_PATH, 'w') as JsonTracksFile:
        json.dump(JsonTracksToDownloadData, JsonTracksFile);


# Requests and downloads a file for a single track. 
def DownloadTrackFile(aCountry, aRegion, aTrackId):

   TRACK_DATA_FILE_PATH = 'TrackData/GaiaGPS/' + aCountry + '/' + aRegion + '/Tracks/' + aTrackId + '.gpx';
   print("Downloading track" + aTrackId);

   DownloadUrl = 'https://www.gaiagps.com/api/objects/track/' + aTrackId + '.gpx';

   Session = requests.session();
   Response = Session.get(DownloadUrl, stream=True);
   StatusCode = Response.status_code;

   print (StatusCode);

   if StatusCode == 200:
       TrackDataFile = open(TRACK_DATA_FILE_PATH, 'wb');
       inside = False
       for Chunk in Response.iter_content(chunk_size=1024):
           inside = True
           if Chunk:
               TrackDataFile.write(Chunk);
       TrackDataFile.close()
       if inside == False: StatusCode = -1;
   return StatusCode


# Main function for downloading tracks of tracklist. 
def DownloadTracks(aCountry, aState, aForce):

    TRACK_DOWNLOAD_CONFIG_FILE_PATH = 'TrackData/GaiaGPS/' + aCountry + '/' + aState + '/TracksToDownload.json';

    #Get data from files.
    with open(TRACK_DOWNLOAD_CONFIG_FILE_PATH, 'r') as JsonTracksFile:
        JsonTracksToDownloadData = json.load(JsonTracksFile);

    print("Get downloads from " + aCountry + "/" + aState + "and force = " + str(aForce));

    # Reset files if force.
    if aForce:

        for TrackId in JsonTracksToDownloadData: 

            JsonTracksToDownloadData[TrackId] = False;

    # ObtainData.
    for TrackId in JsonTracksToDownloadData: 

        if JsonTracksToDownloadData[TrackId] == False:

            DownloadTrackFile(aCountry, aState, TrackId);
            time.sleep(1);
            JsonTracksToDownloadData[TrackId] = True;

    # Update files. 
    with open(TRACK_DOWNLOAD_CONFIG_FILE_PATH, 'w') as JsonTracksFile:
        json.dump(JsonTracksToDownloadData, JsonTracksFile);


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

        ActionsNeedingWebdriver = {
            GET_ROUTES_ACTION_ID,
            GET_TRACKS_ACTION_ID,
            FORCE_GET_ROUTES_ACTION_ID,
            FORCE_GET_TRACKS_ACTION_ID 
        };  

        if ActionsNeedingWebdriver.intersection(set(aActionsToPerform)):
            WebDriver = Login();

        RegionsList = ObtainJsonRegionsList();

        for Country in RegionsList:

            StatesList = RegionsList[Country];

            for State in StatesList:

                if GET_ROUTES_ACTION_ID in aActionsToPerform: 

                    GetRoutesFromRegion(Country, State, WebDriver, False);

                if FORCE_GET_ROUTES_ACTION_ID in aActionsToPerform:

                    GetRoutesFromRegion(Country, State, WebDriver, True);

                if GET_TRACKS_ACTION_ID in aActionsToPerform: 

                    GetTracksFromRoute(Country, State, WebDriver, False);

                if FORCE_GET_TRACKS_ACTION_ID in aActionsToPerform:

                    GetTracksFromRoute(Country, State, WebDriver, True);

                if DOWNLOAD_TRACKS_ACTION_ID in aActionsToPerform:

                    DownloadTracks(Country, State, False); 

                if FORCE_DOWNLOAD_TRACKS_ACTION_ID in aActionsToPerform:

                    DownloadTracks(Country, State, True); 

        if  ActionsNeedingWebdriver.intersection(set(aActionsToPerform)):
            WebDriver.quit(); 


# Downloads tracks from configured region using Gaia GPS. 
def main():

    ActionsToPerform = [];

    while QUIT_ACTION_ID not in ActionsToPerform:
        ActionsToPerform = SelectActions(); 
        PerformActions(ActionsToPerform); 


if __name__ == "__main__": main()