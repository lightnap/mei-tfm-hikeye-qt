
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium import webdriver
import json
import time

# Global variables. 
BOUNDS_FILE_PATH = 'Configuration/LocationConfig.json';
CREDENTIALS_FILE_PATH = 'Configuration/CredentialsConfig.json';
DATA_FOLDER_PATH = 'GroundTruthData/Wikiloc/';
CHROME_DRIVER_PATH = "C:/Users/roger/Documents/Universitat/TFM/BrowserDrivers/chromedriver.exe";


# Reads the bounding box data stored in the corresponding json file.
def ObtainJsonBoundsData():
	JsonFile = open(BOUNDS_FILE_PATH);
	JsonData = json.load(JsonFile);

	BoundingBox = JsonData['BoundingBox'];
	return BoundingBox; 


# Returns a list with the track IDs that will be downloaded. 
def GetTracksId(aBoundingBox):

    NorthLimit = aBoundingBox['NorthLimit'];
    SouthLimit = aBoundingBox['SouthLimit'];
    EastLimit = aBoundingBox['EastLimit'];
    WestLimit = aBoundingBox['WestLimit'];

    TrackIdList = [
    '11544552',
    '122794717',
    '85951581',
    '46348962'
    ];

    return TrackIdList;


# Obtains the suitable credentials by login in to wikilogs.
def Login(): 

    LoginUrl = 'https://www.wikiloc.com/wikiloc/start.do'

    ChromeService = Service(executable_path = CHROME_DRIVER_PATH)
    Driver = webdriver.Chrome(service = ChromeService);

    Driver.maximize_window();

    Driver.get(LoginUrl);

    time.sleep(1); 

    #CookiesButton = Driver.find_element(By.CLASS_NAME, 'btn btn-success cookie-monster__container__content__accept');
    #CookiesButton.click(); 

    LoggedInUrl = "https://www.wikiloc.com/wikiloc/user.do?";

    while not (LoggedInUrl in Driver.current_url):
        print('Waiting For Login');
        time.sleep(1);

    print('LoginSuccessful');

    #Press accept cookies button

    return Driver; 


# Requests and downloads a file for a single track. 
def DownloadTrackFile(aTrackId, aDriver):

    print('Begin DownLoad Of Track' + aTrackId);

    DownloadUrl = 'https://www.wikiloc.com/wikiloc/download.do?id=' + aTrackId;

    aDriver.get(DownloadUrl);

    time.sleep(1);

    FileDownloadLink = aDriver.find_element(By.LINK_TEXT, 'File');
    FileDownloadLink.click();

    time.sleep(1);

    FullFileDownloadLink = aDriver.find_element(By.CSS_SELECTOR, "input[type='radio'][value='original']");
    FullFileDownloadLink.click();

    time.sleep(1);

    DownloadLink = aDriver.find_element(By.ID, 'btn-download-file');
    DownloadLink.click();

    print('Finished download');


def DownloadWikilocTracks():

    BoundingBox = ObtainJsonBoundsData();
    TrackIdList = GetTracksId(BoundingBox);

    Driver = Login();

    time.sleep(5);

    for TrackId in TrackIdList:
        DownloadTrackFile(TrackId, Driver);
        time.sleep(5); 


def main():
    DownloadWikilocTracks();


if __name__ == "__main__": main()