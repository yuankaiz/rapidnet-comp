#include <vector>
#include <utility>
#include <string>

std::vector<std::pair<std::string, float> > getPrediction(std::string file);

std::vector<std::pair<std::string, float> > classifyImage(int clf_id, std::string img_file);

std::vector<std::string> classifyPerson(std::vector<std::string> pattern);

int initClassifier(int clfid);

