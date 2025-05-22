#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <chrono>
#include <atomic>
#include <random>

std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> imageQueue;
const int MAX_IMAGES = 10;
std::atomic<bool> producersDone(false);

std::string generateImage(int id) {
    return "image_" + std::to_string(id);
}

void producer(int id, int numImages) {
    for (int i = 0; i < numImages; ++i) {
        std::string image = generateImage(id * 100 + i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));
        {
            std::unique_lock<std::mutex> lock(mtx);
            imageQueue.push(image);
            std::cout << "[Producer " << id << "] Generated " << image << "\n";
        }
        cv.notify_one();
    }
}

void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {
            return !imageQueue.empty() || producersDone;
        });
        if (!imageQueue.empty()) {
            std::string image = imageQueue.front();
            imageQueue.pop();
            lock.unlock();

            std::cout << "  [Consumer " << id << "] Processing " << image << "...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 300));
            std::cout << "  [Consumer " << id << "] Done with " << image << "\n";
        } else if (producersDone) {
            break;
        }
    }
}

int main() {
    const int NUM_PRODUCERS = 2;
    const int NUM_CONSUMERS = 2;
    const int IMAGES_PER_PRODUCER = 5;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back(consumer, i + 1);
    }

    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back(producer, i + 1, IMAGES_PER_PRODUCER);
    }

    for (auto& t : producers) {
        t.join();
    }

    producersDone = true;
    cv.notify_all();

    for (auto& t : consumers) {
        t.join();
    }

    std::cout << "\nAll images processed successfully.\n";
    return 0;
}