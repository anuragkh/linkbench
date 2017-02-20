#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <ctype.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <algorithm>

std::unordered_map<uint64_t, uint32_t> ops;
std::vector<uint32_t> counts;

uint32_t max = 0;
double mean = 0.0;

void print_usage(const char* exec) {
  fprintf(stderr, "Usage: %s operations-file\n", exec);
}

void process_ops_file(const std::string& file) {
  std::ifstream in(file);
  uint64_t node_id;
  uint32_t shard_id;
  char sep, type;
  uint64_t num_ops = 0;
  while (in >> node_id >> sep >> shard_id >> type && sep == ',') {
    fprintf(stderr, "%llu\t%u\t%c\n", node_id, shard_id, type);
    ops[node_id]++;
    num_ops++;
  }

  fprintf(stderr, "Processed %llu ops; ", num_ops);

  counts.reserve(ops.size());
  double sum = 0.0;
  for (auto op: ops) {
    if (op.first < 10000001)
      counts.push_back(op.second + 1);
    else
      counts.push_back(op.second);
    sum += op.second;
    if (op.second > max)
      max = op.second;
  }
  mean = sum / counts.size();
  std::sort(counts.begin(), counts.end());

  fprintf(stderr, "Max = %u, Mean = %lf\n", max, mean);
}

void output_stats(const std::string& file) {
  std::ofstream mean_out(file + ".mean");
  mean_out << mean << "\n";
  mean_out.close();

  std::ofstream max_out(file + ".max");
  max_out << max << "\n";
  max_out.close();

  std::ofstream count_out(file + ".count");
  for (auto count : counts)
    count_out << count << "\n";
  count_out.close();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    print_usage(argv[0]);
    return 0;
  }

  std::string file = std::string(argv[1]);
  process_ops_file(file);
  // output_stats(file);

  return 0;
}
