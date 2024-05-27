#!/usr/bin/env python3

# This script perform a matching using fmm.
import sys
import csv
import os
import time

sys.path.insert(
    1, "/home/lightnap/src/fmm/matcher/build/python"
)  # TODO: learn how to properly do this.

from fmm import (
    Network,
    NetworkGraph,
    STMATCH,
    STMATCHConfig,
    FastMapMatch,
    UBODTGenAlgorithm,
    UBODT,
    FastMapMatchConfig,
)

UBODT_FILE = "importer/montseny-network/ubodt.txt"
TRACK_DIRECTORY = "importer/tracks_csv"
MATCHES_DIRECTORY = "matches"


def perform_match_on_track(track_wkt, times, fmm_config, model):
    # print(str(wkt))
    result = model.match_wkt(track_wkt, fmm_config)

    # print(result.cpath)
    # return list(result.opath)
    return list(result.cpath)

    # print ('Starting new track:')
    # print ("Opath ",list(result.opath))
    # print ("Cpath ",list(result.cpath))
    # print ("WKT ",result.mgeom.export_wkt())


def perform_match_on_file(input_file_path, output_file_path, fmm_config, model):

    print("\nMatching file" + input_file_path + "...")
    track_file = open(input_file_path, "r")
    match_file = open(output_file_path, "w")

    input_csv = csv.reader(track_file, delimiter=";", quotechar='"')
    output_csv = csv.writer(match_file, delimiter=",", quotechar='"')
    output_csv.writerow(["id", "date", "match"])
    index = 1

    for row in list(input_csv)[1:]:
        match_path = perform_match_on_track(row[1], row[2], fmm_config, model)
        if match_path:
            print("SUCCESS")
            date = int(float(row[2].split(",")[0]))
            match_str = ";".join(str(x) for x in match_path)
            output_csv.writerow([str(index), str(date), match_str])

        else:
            print("NO MATCH FOUND")

        index = index + 1

    track_file.close()
    match_file.close()


def main():

    network = Network("importer/montseny-network/edges.shp", "fid", "u", "v")

    print(
        "Nodes {} edges {}".format(network.get_node_count(), network.get_edge_count())
    )
    graph = NetworkGraph(network)

    # Can be skipped if you already generated an ubodt file
    ubodt_gen = UBODTGenAlgorithm(network, graph)
    status = ubodt_gen.generate_ubodt(
        UBODT_FILE, delta=3000, binary=False, use_omp=True
    )
    print(status)

    # Read UBODT
    start_time = time.time()
    ubodt = UBODT.read_ubodt_csv(UBODT_FILE)
    end_time = time.time()
    print("Time to read UBODT:", end_time - start_time)


    # Create FMM model
    model = FastMapMatch(network, graph, ubodt)

    # Define map matching configurations
    k = 8
    radius = 350
    gps_error = 55
    fmm_config = FastMapMatchConfig(k, radius, gps_error)

    directory = os.fsencode(TRACK_DIRECTORY)

    start_time = time.time()

    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        track_csv_file_path = TRACK_DIRECTORY + "/" + filename
        matches_file_path = MATCHES_DIRECTORY + "/" + filename.split(".")[0] + ".match"
        perform_match_on_file(track_csv_file_path, matches_file_path, fmm_config, model)

    end_time = time.time()
    print("Time to do the matches: ", end_time - start_time)


main()
