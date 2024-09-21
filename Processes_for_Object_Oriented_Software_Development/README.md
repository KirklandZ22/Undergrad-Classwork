# Processes for Object-Oriented Software Development - Projects

The course emphasized the full software development lifecycle, focusing on building robust web applications with both front-end and back-end components, utilizing industry-standard tools, and adhering to agile methodologies.

## Project 1: Personal Contact Manager (LAMP Stack)

### Overview
This project involved building a web-based personal contact manager using the **LAMP stack** (Linux, Apache, MySQL, PHP). The primary goal was to implement a system where users can securely manage their personal contacts, with features like user registration, login, and contact management (add, search, edit, and delete). JSON was used for communication between the client and server.

### Key Features:
- **User Authentication**: New users can sign up, and existing users can log in to manage their contacts. All user data is stored securely in a remote MySQL database.
- **Contact Management**: Logged-in users can add new contacts, search through their contacts using a search API, edit details, and delete them. Contacts are user-specific and not shared between accounts.
- **Search API**: The system includes a dedicated search API, and contacts are not cached client-side to ensure real-time data fetching from the server.
- **Lazy Loading API**: Allows for continuous request to load more users while scrolling in order to prevent a large amount of data being loaded at one time. 

### Technologies:
- **Frontend**: HTML, CSS, JavaScript
- **Backend**: PHP, Apache
- **Database**: MySQL

## Project 2: Gaming Gear eCommerce Platform (MERN Stack)

### Overview
For the second project there was no required theme for the project. Our team decided to create a sample eCommerce website where users can buy and sell gaming gear. The project used the **MERN stack** (MongoDB, Express, React, Node.js) to implement both the web and mobile interfaces. This project introduced more advanced full-stack development techniques and expanded the functionality to include a **mobile application** alongside the web client.

### Key Features:
- **Product Listings**: The platform allows users to browse gaming gear, with each product having detailed descriptions, prices, and availability. Products are stored in a remote MongoDB database.
- **User Authentication**: Users can create accounts, log in, and manage their profiles. Only registered users can add items to their cart or make purchases.
- **Shopping Cart**: Users can add items to their shopping cart, update quantities, and proceed to checkout. This data is dynamically updated and stored both server-side and in the database.
- **Mobile Application**: A companion mobile app was developed alongside the web client, providing seamless access to the platform on mobile devices.
- **Lazy Loading API**: Allows for continuous request to load more users while scrolling in order to prevent a large amount of data being loaded at one time. 

  
### Technologies:
- **Frontend (Web)**: React.js, HTML, CSS, JavaScript
- **Backend**: Node.js, Express.js
- **Database**: MongoDB
- **Mobile App**: React Native

