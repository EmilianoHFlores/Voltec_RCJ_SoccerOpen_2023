class OpenMV {
    constructor (width = 320, height = 240) {
        this.width = 320;
        this.height = 240;
    }

    data(queryString = '') {
        let query = [];
        let resultString = '';
        for (let i = 0; i < queryString.length; i += 2) {
            query.push(queryString.slice(i, i + 2));
        }
        for (let i = 0; i < query.length; i ++) {
            switch (query[i].toLowerCase()) {
            case 'ox':
                resultString += `-ox:${this.#orangeX()}`;
                break;
            case 'oy':
                resultString += `-oy:${this.#orangeY()}`;
                break;
            case 'bx':
                resultString += `-bx:${this.#blueX()}`;
                break;
            case 'by':
                resultString += `-by:${this.#blueY()}`;
                break;
            case 'yx':
                resultString += `-yx:${this.#yellowX()}`;
                break;
            case 'yy':
                resultString += `-yy:${this.#yellowY()}`;
                break;
            }    
        }
        return resultString;
    }

    #orangeX () { return Math.floor(Math.random() * this.width) }
    #orangeY () { return Math.floor(Math.random() * this.height) }
    #blueX () { return Math.floor(Math.random() * this.width) }
    #blueY () { return Math.floor(Math.random() * this.height) }
    #yellowX () { return Math.floor(Math.random() * this.width) }
    #yellowY () { return Math.floor(Math.random() * this.height) }
}

module.exports = OpenMV;
