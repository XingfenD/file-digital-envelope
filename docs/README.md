# File Digital Envelope

Cryptography experiment project, Class of 2022, School of Cyber Science and Engineering, Wuhan University.

## Introduction to File Digital Envelope

### What is a File Digital Envelope?

A digital envelope is a technology that combines the advantages of both symmetric encryption and asymmetric encryption for secure information transmission.

The file digital envelope leverages the high speed and security of symmetric encryption while also utilizing the convenient key management of asymmetric encryption.

A digital envelope generally consists of two parts: a key encrypted with an asymmetric algorithm and ciphertext encrypted with a symmetric algorithm.

The key part is the symmetric key, which is used to decrypt the ciphertext.

In simpler terms, it is like having two locked boxes: one containing the key to open the other. You possess the key to unlock the first box.

For clarity, these two boxes will be referred to as the "key box" and the "ciphertext box." You hold the key to the key box.

### Why take this approach?

As mentioned earlier, the two parts of the digital envelope use different encryption algorithms.

The key box is encrypted using an asymmetric algorithm, while the ciphertext box is encrypted using a symmetric algorithm.

This allows the file digital envelope to benefit from the convenience of asymmetric encryption and the efficiency of symmetric encryption.

(Encrypting the key using an asymmetric algorithm is much faster than encrypting the entire data.)

## `fde` Command-Line Tool

This project implements a command-line tool, `fde`, for generating and parsing file digital envelopes. It also includes a binary file format protocol specifically designed for `fde`.

`fde` supports multiple symmetric and asymmetric encryption algorithms to generate and parse `fde` files.

### Using the `fde` Command-Line Tool

#### Recognized Command-Line Parameters for `fde`

| Option | Description                                                                                                               | Requirement |
| ------ | ------------------------------------------------------------------------------------------------------------------------- | ----------- |
| `-f`   | Input file path                                                                                                           | Required    |
| `-o`   | Output file path (defaults to the replace the extension with the original file name stored in the fde file)               | Optional    |
| `-k`   | Path to the asymmetric key file                                                                                           | Required    |
| `-m`   | Mode of operation:<br>`enc` - Encrypt the input file into a digital envelope<br>`dec` - Decrypt the digital envelope file | Required    |
| `-h`   | Display help                                                                                                              | Optional    |
| `-a`   | Specify asymmetric encryption algorithm (e.g., `rsa`)                                                                     | Optional    |
| `-s`   | Specify symmetric encryption algorithm (e.g., `sm4`)                                                                      | Optional    |

#### Dependencies and Mutual Exclusions of `fde` Command-Line Parameters
