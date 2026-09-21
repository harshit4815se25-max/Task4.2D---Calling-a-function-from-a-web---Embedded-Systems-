import { initializeApp }
from "https://www.gstatic.com/firebasejs/10.12.2/firebase-app.js";

import {
  getAuth,
  signInWithEmailAndPassword
}
from "https://www.gstatic.com/firebasejs/10.12.2/firebase-auth.js";

import {
  getDatabase,
  ref,
  set,
  onValue
}
from "https://www.gstatic.com/firebasejs/10.12.2/firebase-database.js";


const firebaseConfig = {
  apiKey: "AIzaSyCgiG7FBOJCz3XgcXXr1_LnDwfMrRSLdjA",
  authDomain: "sit210-dtask.firebaseapp.com",
  databaseURL: "https://sit210-dtask-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "sit210-dtask",
  storageBucket: "sit210-dtask.firebasestorage.app",
  messagingSenderId: "531718167749",
  appId: "1:531718167749:web:c7397e71ef490bf402a4ac"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const db = getDatabase(app);

const email = "harshitjindal348@gmail.com";
const password = "harshit";

const status = document.getElementById("status");

async function connectFirebase() {
  try {
    await signInWithEmailAndPassword(
      auth,
      email,
      password
    );

    status.textContent = "Connected";

    listenToDatabase();

  } catch (error) {
    console.error("Firebase error:", error);
    console.error("Error code:", error.code);
    console.error("Error message:", error.message);

    status.textContent =
      "Connection failed: " + error.code;
  }
}

function listenToDatabase() {
  onValue(ref(db), function(snapshot) {

    const data = snapshot.val();

    if (!data) return;

    document.getElementById("livingroom").checked =
      data.livingroom === true;

    document.getElementById("bathroom").checked =
      data.bathroom === true;

    document.getElementById("closet").checked =
      data.closet === true;
  });
}

document.getElementById("livingroom").onchange =
  function() {
    set(
      ref(db, "livingroom"),
      this.checked
    );
  };

document.getElementById("bathroom").onchange =
  function() {
    set(
      ref(db, "bathroom"),
      this.checked
    );
  };

document.getElementById("closet").onchange =
  function() {
    set(
      ref(db, "closet"),
      this.checked
    );
  };

connectFirebase();