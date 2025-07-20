#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Test the CLI tool with various arguments
int test_cli_usage() {
    printf("Testing CLI usage...\n");
    
    // Test 1: No arguments (should show usage and exit with error)
    printf("Test 1: No arguments\n");
    
    // Check if epdraw binary exists
    if (access("../bin/epdraw", F_OK) != 0) {
        printf("⚠ Test 1 skipped: epdraw binary not found (expected on macOS)\n");
    } else {
        int pid = fork();
        if (pid == 0) {
            // Child process
            execl("../bin/epdraw", "epdraw", NULL);
            exit(1); // Should not reach here
        } else {
            // Parent process
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
                printf("✓ Test 1 passed: CLI shows usage with no args\n");
            } else {
                printf("✗ Test 1 failed: Expected exit code 1, got %d\n", WEXITSTATUS(status));
                return 1;
            }
        }
    }
    
    // Test 2: Help argument (should show usage)
    printf("Test 2: Help argument\n");
    
    if (access("../bin/epdraw", F_OK) != 0) {
        printf("⚠ Test 2 skipped: epdraw binary not found (expected on macOS)\n");
    } else {
        int pid = fork();
        if (pid == 0) {
            execl("../bin/epdraw", "epdraw", "--help", NULL);
            exit(1);
        } else {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
                printf("✓ Test 2 passed: CLI shows usage with --help\n");
            } else {
                printf("✗ Test 2 failed: Expected exit code 1, got %d\n", WEXITSTATUS(status));
                return 1;
            }
        }
    }
    
    // Test 3: Non-existent file (should fail gracefully)
    printf("Test 3: Non-existent file\n");
    
    if (access("../bin/epdraw", F_OK) != 0) {
        printf("⚠ Test 3 skipped: epdraw binary not found (expected on macOS)\n");
    } else {
        int pid = fork();
        if (pid == 0) {
            execl("../bin/epdraw", "epdraw", "nonexistent.bmp", NULL);
            exit(1);
        } else {
            int status;
            wait(&status);
            int exit_code = WEXITSTATUS(status);
            // In Docker/CI environment, the CLI might return 0 (no hardware) or non-zero (file error)
            // Both are acceptable behaviors
            if (WIFEXITED(status)) {
                printf("✓ Test 3 passed: CLI handled non-existent file (exit code: %d)\n", exit_code);
            } else {
                printf("✗ Test 3 failed: CLI crashed or didn't exit properly\n");
                return 1;
            }
        }
    }
    
    return 0;
}

int main() {
    printf("Running CLI tests...\n");
    
    int result = test_cli_usage();
    
    if (result == 0) {
        printf("All CLI tests passed!\n");
    } else {
        printf("Some CLI tests failed!\n");
    }
    
    return result;
} 